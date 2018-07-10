#ifndef __COMMON_H__
#define __COMMON_H__

/** @page arpc �첽RPCϵͳ.

arpcϵͳ( asynchronous remote procedure calling )��һ��������CORBA��ϵͳ����������װ�����󷽷����ü��𣬷���������Э��ı�д��

@section aprc_ref �ο�ϵͳ
- google protocol buffers.
- CORBA

��Щʵ�ֶ��ܳ����ұ��㷺ʹ�ã�������Щʵ�ֶ�����Ϸ��˵��һЩȱ��:
- ���ڸ��ӣ���������ϷӦ��
- Ч������
- ���Ѷ�idl�����Զ�����չ 


@section desc ϵͳ����
arpcϵͳ�����Ŀ����һ������rpcϵͳ����Ҫ���rpc��������͵��ù��ܣ��������ܲ��ڿ��Ƿ�Χ�ڡ�

@par arpc�Ĺ���
- ��Ӧ����ͨѶ��Ϣ�Ľ������������е���Ϣ��������������RPC�������Զ����ɣ������ֶ���д�����������ĸ���
- ����Ϣ���޸Ĺ���
- RPC�����Ĵ����ʵ�ַֿ���ʹRPCϵͳ�����ڸ��ִ���Э���Լ�IOģ����ʹ��
- ����ͨ��RPC����Ĳ���������ʵ�ִ��룬��������Զ����Թ��ߣ���Ϣlog���ߵȵ�
Ϊ�˴ﵽϵͳ��Ƽ�����Ŀ�꣬arpc���еĵ���ȫ�������첽������ÿ������ֻ��ʹ�ô����������û�д��������ͷ���ֵ��
��ʹ��arpcϵͳʱ�����Խ�����˫�������ǻ����첽���������¼�����

@par
aprcϵͳ��Ҫ��һ��idl��������һ������ʱ�����
@image html arpc.png
@see Sepcification ProtocolWriter ProtocolReader 

@subsection aprc_stub Service Stub
arpcc �����һ��service�����������service���÷�stub�ࡣ
stub��Ҫ����һ����Ա�������ý������࣬������������д�뵽���õ� ProtocolWriter �С�
ʹ���߿���ͨ������ ProtocolWriter ��ʵ������Э������ݵĴ��书�ܡ� 

@subsection proxy Service Proxy
arpcc �����һ��service�����������service�����÷���proxy�ࡣ
ͨ������proxy��ȫ��Ӧ�ú���dispatch��������һ�� ProtocolReader ʵ����һ��proxy������ʵ����
���Խ�stub�������ݽ��ɶ����proxy�����Ա�����ĵ��á�
ʹ������Ҫ����proxy��ʵ�ִ����Ա�������幦�ܡ�
@note proxy ֻ�ṩ��๦�ܣ������ṩ���崦�����Ķ�λ��proxy������Ķ�λӦ����ʹ��������ʵ�֡�


@section safety arpc��ȫ��
������Ͷ��а�ȫ����(��game client),Ӧ�ö�һЩ�ؼ���������á�
��Щ����ֻ���ڽ��ն����ɼ����롣

@par ��̬��С����
������Ҫʹ�ö�̬��С������method�������ܵ�������
��̬�������� array �� string.
���ʹ�ã�����趨array��string�������󳤶�.

*/

#include <string>

typedef signed long long	S64;
typedef unsigned long long	U64;
typedef double				F64;
typedef float				F32;
typedef signed int			S32;
typedef unsigned int		U32;
typedef signed short		S16;
typedef unsigned short		U16;
typedef signed char			S8;
typedef unsigned char		U8;
typedef bool				B8;

typedef std::string			STRING;

/** ��ʾһ��enum������. */
typedef unsigned char EnumSize;


#include <vector>
#include <string>
#include <algorithm>



#endif//__COMMON_H__
