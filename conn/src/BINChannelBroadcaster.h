#ifndef __BINChannelBroadcaster_h__
#define __BINChannelBroadcaster_h__

#include "ProtocolWriter.h"
#include "ChannelBroadcaster.h"


/** ����BINЭ��� ChannelBroadcaster.

	BINChannelBroadcaster �� һ�� BIN_STUB ��һ�� ChannelBroadcaster 
	�Ĺ��ܽ������ϣ�ʹ���߿���ͨ������ BIN_STUB �ĺ���������ε��õ����ݹ㲥��
	���������е� channel��

	@todo �д��Ż�����setΪ��ʱӦ�þ������ټ�����.
*/
template<class BIN_STUB, class CHANNEL>
class BINChannelBroadcaster :
	public ChannelBroadcaster<CHANNEL>,
	public BIN_STUB,
	public BIN_ProtocolWriter
{
public:
	BINChannelBroadcaster() {}

private:
	/** Stub events. */
	virtual BIN_ProtocolWriter* methodBegin()
	{
		this->initSendingData();
		return this;
	}
	virtual void methodEnd()
	{
		this->flushSendingData();
	}
	/** BIN_ProtocolWriter interface. */
	virtual void write(const void* data, size_t len)
	{
		this->fillSendingData((void*)data, len);
	}
};


#endif//__BINChannelBroadcaster_h__
