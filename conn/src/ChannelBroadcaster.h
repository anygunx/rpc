#ifndef __ChannelBroadcaster_h__
#define __ChannelBroadcaster_h__

#include "ChannelConnection.h"
#include <set>
#include <map>

/** ���һ�� CHANNEL ���͹㲥��Ϣ�Ĺ�����.
	
	ChannelBroadcaster ����һ�� CHANNEL �ļ��ϣ����ͨ�� ChannelBroadcaster 
	������Ϣ�ķ��ͣ������Ϣ��㲥��������������е� CHANNEL ����

	ChannelBroadcaster ������ ChannelConnection �����ݹ㲥�ӿ��ϡ�
*/
template<class CHANNEL>
class ChannelBroadcaster 
{
public:
	ChannelBroadcaster()
	{
	}

	virtual ~ChannelBroadcaster()
	{
	}

	/** ���һ��channel����.
		@warning ���channel���������Ч�������ڱ�ChannelBroadcaster���������
				 ���ܱ��ر�.
	*/
	void addChannel(CHANNEL* ch)
	{
		ACE_ASSERT(ch->isValid());
		ChannelConnection* conn = ch->getConn();
		ACE_ASSERT(conn);
		// ���Ҳ�������channel��Ӧ��connection.
		std::pair<typename ConnToChanMap::iterator, bool> r = 
			connToChans_.insert(std::pair<ChannelConnection*, ChannelSet>(conn, ChannelSet()));
		r.first->second.insert(ch);
	}

	/** �Ƴ�һ��channel����. */
	void removeChannel(CHANNEL* ch)
	{
		ACE_ASSERT(ch->isValid());
		ChannelConnection* conn = ch->getConn();
		ACE_ASSERT(conn);
		typename ConnToChanMap::iterator iter = connToChans_.find(conn);
		ACE_ASSERT(iter != connToChans_.end());
		int r = iter->second.erase(ch);
		ACE_ASSERT(r == 1);
		if(iter->second.size() == 0)
			connToChans_.erase(iter);
	}

	/** ������е�channel����. */
	void clearChannels()
	{
		connToChans_.clear();
	}

	/** ׼���㲥����. 
		�˲���Ϊ�����е�ÿ��connection׼����channel���ϣ��ȴ����������� fillSendingData 
		���㲥����.
	*/
	void initSendingData()
	{
		for(typename ConnToChanMap::iterator iter = connToChans_.begin(); iter != connToChans_.end(); ++iter)
		{
			ChannelConnection* conn = iter->first;
			conn->initBCSendingData((std::set<Channel*>&)iter->second);
		}
	}

	/** ���㲥����. 
		�˲���Ϊ�����е�ÿ��connection���ô��㲥�����ݣ��ȴ��������� flushSendingData
		���͹㲥����.
	*/
	void fillSendingData(void* data, size_t size)
	{
		for(typename ConnToChanMap::iterator iter = connToChans_.begin(); iter != connToChans_.end(); ++iter)
		{
			ChannelConnection* conn = iter->first;
			conn->fillSendingData(data, size);
		}
	}

	/** ���͹㲥����. 
		�˲���Ϊ�����е�ÿ��connection�����յ����ݷ��ͣ��㲥����.
	*/
	void flushSendingData()
	{
		for(typename ConnToChanMap::iterator iter = connToChans_.begin(); iter != connToChans_.end(); ++iter)
		{
			ChannelConnection* conn = iter->first;
			conn->flushSendingData();
		}
	}

	typedef std::set<CHANNEL*> ChannelSet;
	typedef std::map<ChannelConnection*, ChannelSet>	ConnToChanMap;
	ConnToChanMap	connToChans_;
};

#endif//__ChannelBroadcaster_h__
