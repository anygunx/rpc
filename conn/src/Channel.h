#ifndef __Channel_h__
#define __Channel_h__

#include <cstddef>

class ChannelConnection;

/** ��������ʵ�� ChannelConnection ֮�ϵ�һ������ͨѶ����.

	����ʹ������˵�� Channel ���Ա�������һ�� Connection һ������һ������
	��˫������ͨѶ�ܵ�������ͨ�� *SendingData() ��Զ˷������ݣ���ͨ������ 
	handleReceived ����Զ˷��͹��������ݡ�

	�ڴ���㣬���е���ЩChannel ����һ�� ChannelConnection ���󣬽�������
	���䡣ÿһ��Channel ������һ��guid�������ڴ���������б�ʶ�������Լ���
	��Ϣ.
*/
class Channel
{
public:
	friend class ChannelConnection;

	/** Channel ��Ĭ�Ϲ����û����Ч�� ChannelConnection ��֮��������Ҫͨ
		�� ChannelConnection ������ģʽ��֮���й���.
	*/
	Channel();

	/** Channel �������������е���close.
	*/
	virtual ~Channel();

	/** ��õ�ǰ������ ChannelConnection.
		@return NULL ��ʾ��ǰû�й���.
	*/
	ChannelConnection* getConn()	{ return conn_; }

	/** ������ Channel ��ǰ�Ƿ���Ч.
		����� Channel ��һ�� ChannelConnection ��Ч����������true
	*/
	bool isValid();

	/** ��ʼ����Ҫ���͵�����. */
	void fillBegin();
	/** �����Ҫ���͵�����. */
	void fill(void* data, size_t size);
	/** ���ͱ���������. */
	void fillEnd();

	/** �ӹܵ�����һ�˽��յ�����.
		��������Ҫ��������ӿڣ�������յ�������
		@param data ����ָ��.
		@param size ���ݴ�С.
		@return ���ش����Ƿ�ɹ�.
	*/
	virtual bool handleReceived( void* data, size_t size )	{ return true; }
	virtual bool handleConnect(){return true;}
	virtual bool handleClose();

protected:
	unsigned int			guid_;
	ChannelConnection*	conn_;
};

#endif//__Channel_h__
