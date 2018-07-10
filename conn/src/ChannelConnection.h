#ifndef __ChannelConnection_h__
#define __ChannelConnection_h__

#include "Connection.h"
#include <map>
#include <vector>
#include <set>

class Channel;


/** Channel ʹ�õ����紫��������.

	ChannelConnection ����� Connection �������߱��� Connection �Ļ�
	�����紫�书�ܡ� ChannelConnection ������ Channel �����ݺ�ȫ�����ݣ�
	������������Լ��� Channel ����

	@par Channel����ģʽ�����ӵĽ���.
	ChannelConnection ���Ա���ʼ��Ϊ���ֻ����ģʽ:
	- Channel connector:
	  ֻ����Զ˷���һ��Channel���������ӣ�������ܶԶ�channel�����������ڴ�ģ
	  ʽ�£�ͨ�� initChannelConnData() ��������һ��channel���������ӣ�Ȼ�����ͨ
	  �� fillChannelConnData() ��������ʼ�������ݡ������� flushChannelConnData()
	  �����������ݡ�����һ����Ч��channel���Ӿͽ����ã�������ʹ����� channel ��
	  ��˫������ݴ���.
	- Channel acceptor: 
	  ֻ�ܽ��ܶԶ˵�channel�������󣬲�����Զ˷���channel�������ӡ��ڴ�ģʽ�£�
	  ʹ������Ҫͨ������������ makeChannel() �����������������ӵ� Channel ��
	  �󣬲����� acceptChannel() �������������Ч��channel������г�ʼ����Ȼ��ʹ
	  �����channel ����˫������ݴ���.
	
	@par ���ӳ�ʼ������.
	�����ӷ�������ʱ���Է���һЩ�Զ���ĳ�ʼ�����ݣ���Щ���ݻ�������Э��һͬ��
	�ﱻ���ӷ������ڳ�ʼ�������ӷ���channel.

	@par Channel���ӵĹر�.
	����ʹ�ú���ģʽ������channel���Ӷ�����ͨ�� closeChannel() ��������رա��ر�
	������ʹ�����channel���к�����ͨѶ�� closeChannel() ֻ�ڱ��˽�� Channel
	�� ChannelConnection �Ĺ����ԣ�������֪ͨ�Զ����channel�Ѿ��رա�
	��ȷ��channel�رշ�ʽӦ����ͨ���߲��Э��Լ���������˸��Թرն�Ӧ��channel��
	��������Ӧ����:
	- A����һ����Ϣ��B
	- A�ر�channel
	- B�յ���Ϣ��ر�channel.

	@par channel����.
	����Ӧ�õ��� initChannelSendingData() ������ʼ��Ҫ���͵����ݣ������� fillSendingData()
	���������ݽ�����䣬������ flushSendingData���з��͡�����ķ��͹����Ѿ���
	��װ�� Channel �С�
	Ӧ������ Channel::handleReceived() ����������յ���channel���ݡ�

	@par ȫ������.
	����Ӧ�õ��� initGlobalSendingData() ������ʼ��Ҫ���͵����ݣ������� fillSendingData()
	���������ݽ�����䣬������ flushSendingData���з��͡�
	Ӧ������ handleGlobalData() ����������յ���ȫ�����ݡ�

	@par ���ݹ㲥
	�����Ҫ��һ������ͬʱ�������� Channel ����Ӧ��ʹ�� ChannelConnection 
	�Ĺ㲥����.ChannelConnection ֧����һ�� Channel �㲥���ݡ�ʹ�ù㲥��
	����Ч��С���ݵĸ��ƴ�����ѹ����Ҫ��������������Ӷ���Ч�����Ч�ʡ�
	���ڹ㲥���ݵķ��ͣ�����Ӧ�õ��� initBCSendingData() ������ʼ��Ҫ���͵����ݣ�
	������ fillSendingData() ���������ݽ�����䣬������ flushSendingData���з�
	�͡�
*/
class ChannelConnection : public Connection
{
public:
	/** Э������. */
	enum ProtocolType
	{
		/** Channel��������Э��. */
		PT_ChanConnect,
		/** Channel�ر�����Э��*/
		PT_ChanDisconnect,
		/** Channel����Э��. */
		PT_ChanData,
		/** Channel�㲥����. */
		PT_ChanBCData,
		/** ��Channelȫ������Э��. */
		PT_GlobalData,
	};


	/** ���캯��.
		@param isConnector ��� ChannelConnection �� connector ���� acceptor.
		@param rbSize ����buffer��С
		@param sbSize ����buffer��С
		@param bktSize hash bucket size, power of 2(default = 4096).
	*/
	ChannelConnection(
		bool isConnector, 
		size_t rbSize = 0XFFFF, 
		size_t sbSize = 0XFFFF,
		size_t bktSize = 12); 
	/** ��������.
		�����connection����ʱ��������رպ��������е�channel���������Ӧ�ý�����
		�������ദ��.
	*/
	virtual ~ChannelConnection();

	/** ��Զ˷���һ�� Channel �����ӣ���׼��ͨ�� fillChannelConnData �����
		������. 
		@note ֻ��connector�ſ��Ե����������.
		@param c ��Ҫ�������ӵ� Channel ����.������channel�Ѿ����ӵ���Ч��
				 connection������Զ��Ͽ���
	*/
	void connect(Channel* c);
	
	/** �Ͽ�һ�� Channel ������.
		����������һ�� Channel ����� ChannelConnection ����ϵ.
		@note ������������󣬿��԰�ȫ��ɾ�� Channel ����.
		@note connector �� acceptor �����Ե���.
		@param c ��Ҫ�Ͽ��� Channel ����.��������Ҫ��֤��� Channel ����
				 ��ǰ������״̬.
	*/
	void disconnect(Channel *c);
	/** ������ӳ�ʼ����. */
	void fill(void* data, size_t size);
	/** ����������Ϣ. */
	void flush();

	/** ������һ����Ч�� Channel ����.
		������ʼ��Ϊ acceptor ģʽ�󣬵���������ʼ����һ����Ч�� Channel ����
		����������������ʱ��� Channel �����Ѿ������ ChannelConnection
		����������ϵ�������ڴ˴�����һ���ĳ�ʼ������������ͨ�����channel ����
		��Ϣ�����������channel����.
	*/
	virtual Channel* accept(){ ACE_ASSERT(0); return NULL;}
	
	/** ���������ӹ�������� Channel �����б�. */
	void getAllChannels(std::vector<Channel*>& channels);

	/** ��ʼ����һ��channel������. 
		�����Ҫͨ��һ��channel�������ݣ�����Ӧ�õ��� startChannelData ��Ȼ��ͨ��
		fillSendBuffer���������룬������ flushData �����յķ���.
	*/
	void initChannelSendingData(Channel* c);
	/** ��ʼ����һ��ȫ������. 
		�����Ҫ����ȫ�����ݣ�����Ӧ�õ��� startGlobalData ��Ȼ��ͨ��
		fillSendBuffer���������룬������ flushData �����յķ���.
	*/
	void initGlobalSendingData();
	/** ��ʼ����һ���㲥����. 
		�����Ҫ���͹㲥���ݣ�����Ӧ�õ��� startBCData ��Ȼ��ͨ��
		fillSendBuffer���������룬������ flushData �����յķ���.
	*/
	void initBCSendingData(std::set<Channel*>& channels);
	/** ������������. */
	void fillSendingData(void* data, size_t size);
	/** ���������ݽ������շ���. */
	void flushSendingData();

	/** ������յ��ķ�channel����.
		@param data ����ָ��.
		@param size ���ݴ�С.
		@return ���ش����Ƿ�ɹ�.
	*/
	virtual bool handleGlobalData(void* data, size_t size);

protected:
	/** ������յ�������.
		����channelЭ��,����ָ���� Channel ����������.
	*/
	virtual int handleReceived(void* data, size_t size);

	/** ���һ��channel. */
	bool addChannel(unsigned int key, Channel* value);
	/** �Ƴ�һ��channel. */
	bool removeChannel(unsigned int key);
	/** ����guid����һ�� Channel ����. */
	Channel* findChannel( unsigned int key );
	unsigned int getBucketSize()			{ return 1<<bktSize_; }
	unsigned int getHash(unsigned int key)	{ return key & ( 0XFFFFFFFF>>(32-bktSize_) ); }

	bool			isConnector_;
	unsigned int	guidGen_;
	char*			msgLen_;

	struct Node
	{
		Node(unsigned int k, Channel* c, Node* n): key_(k), chan_(c), next_(n) {}
		unsigned int	key_;
		Channel*	chan_;
		Node*			next_;
	};
	Node**			nodes_;
	unsigned int	bktSize_;
	unsigned int	nodeNum_;
};

#endif//__ChannelConnection_h__
