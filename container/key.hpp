
#ifndef _CXXREDIS_OP_KEY_HPP_
#define _CXXREDIS_OP_KEY_HPP_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <string>

namespace redis {

	class client;
	class reply;

	namespace container {

		class key {

		public:
			key(client& cli);
			~key();

			/**
			* ɾ��������һ������ key ��
			* �����ڵ� key �ᱻ���ԡ�
			*
			* ���ð汾��>= 1.0.0
			*
			* ʱ�临�Ӷȣ�O(N)�� N Ϊ��ɾ���� key ��������
			* ɾ�������ַ������͵� key ��ʱ�临�Ӷ�ΪO(1)��
			* ɾ�������б������ϡ����򼯺ϻ��ϣ�����͵� key ��ʱ�临�Ӷ�ΪO(M)�� M Ϊ�������ݽṹ�ڵ�Ԫ��������
			*
			* ����ֵ����ɾ�� key ��������
			*/
			long long del(const std::list<std::string>& keys);
			/**
			* ���л����� key �������ر����л���ֵ��ʹ�� RESTORE ������Խ����ֵ�����л�Ϊ Redis ����
			*
			* ���л����ɵ�ֵ�����¼����ص㣺
			* ������ 64 λ��У��ͣ����ڼ����� RESTORE �ڽ��з����л�֮ǰ���ȼ��У��͡�
			* ֵ�ı����ʽ�� RDB �ļ�����һ�¡�
			* RDB �汾�ᱻ���������л�ֵ���У������Ϊ Redis �İ汾��ͬ��� RDB ��ʽ�����ݣ���ô Redis ��ܾ������ֵ���з����л�������
			* ���л���ֵ�������κ�����ʱ����Ϣ��
			*
			* ���ð汾��>= 2.6.0
			*
			* ʱ�临�Ӷȣ�
			* ���Ҹ������ĸ��Ӷ�Ϊ O(1) ���Լ��������л��ĸ��Ӷ�Ϊ O(N*M) ������ N �ǹ��� key �� Redis ������������� M ������Щ�����ƽ����С��
			* ������л��Ķ����ǱȽ�С���ַ�������ô���Ӷ�Ϊ O(1) ��
			*
			* ����ֵ��
			* ��� key �����ڣ���ô���� nil ��
			* ���򣬷������л�֮���ֵ��
			*/
			reply dump(const std::string& k);

			/**
			* �����л����������л�ֵ���������͸����� key ������
			* ���� ttl �Ժ���Ϊ��λΪ key ��������ʱ�䣻��� ttl Ϊ 0 ����ô����������ʱ�䡣
			*
			* RESTORE ��ִ�з����л�֮ǰ���ȶ����л�ֵ�� RDB �汾������У��ͽ��м�飬
			* ��� RDB �汾����ͬ�������ݲ������Ļ�����ô RESTORE ��ܾ����з����л���������һ������
			*

			* ���ð汾��>= 2.6.0
			*
			* ʱ�临�Ӷȣ�
			* ���Ҹ������ĸ��Ӷ�Ϊ O(1) ���Լ����з����л��ĸ��Ӷ�Ϊ O(N*M) ������ N �ǹ��� key �� Redis ������������� M ������Щ�����ƽ����С��
			* ���򼯺�(sorted set)�ķ����л����Ӷ�Ϊ O(N*M*log(N)) ����Ϊ���򼯺�ÿ�β���ĸ��Ӷ�Ϊ O(log(N)) ��
			* ��������л��Ķ����ǱȽ�С���ַ�������ô���Ӷ�Ϊ O(1) ��
			*
			* ����ֵ��
			* ��������л��ɹ���ô���� OK �����򷵻�һ������
			*/
			void restore(const std::string& k, time_t ttl, const std::string& serializedValue);

			/**
			* ������ key �Ƿ���ڡ�
			*
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			* ����ֵ���� key ���ڣ����� 1 �����򷵻� 0 ��
			*/
			bool exists(const std::string& k);

			/**
			* Ϊ���� key ��������ʱ�䣬�� key ����ʱ(����ʱ��Ϊ 0 )�����ᱻ�Զ�ɾ����
			* �� Redis �У���������ʱ��� key ����Ϊ����ʧ�ġ�(volatile)��
			*
			* ����ʱ�����ͨ��ʹ�� DEL ������ɾ������ key ���Ƴ������߱� SET �� GETSET ���д(overwrite)��
			* ����ζ�ţ����һ������ֻ���޸�(alter)һ��������ʱ��� key ��ֵ��������һ���µ� key ֵ������(replace)���Ļ�����ô����ʱ�䲻�ᱻ�ı䡣
			*
			* ����˵����һ�� key ִ�� INCR �����һ���б����� LPUSH ������߶�һ����ϣ��ִ�� HSET �����������������޸� key ����������ʱ�䡣
			* ��һ���棬���ʹ�� RENAME ��һ�� key ���и�������ô������� key ������ʱ��͸���ǰһ����
			*
			* RENAME �������һ�ֿ����ǣ����Խ�һ��������ʱ��� key ��������һ��������ʱ��� another_key ��
			* ��ʱ�ɵ� another_key (�Լ���������ʱ��)�ᱻɾ����Ȼ��ɵ� key �����Ϊ another_key ��
			* ��ˣ��µ� another_key ������ʱ��Ҳ��ԭ���� key һ����
			* 
			* ʹ�� PERSIST ��������ڲ�ɾ�� key ������£��Ƴ� key ������ʱ�䣬�� key ���³�Ϊһ�����־õġ�(persistent) key ��
			*
			* ���ð汾��>= 1.0.0
			*
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* ���óɹ����� 1 ��
			* �� key �����ڻ��߲���Ϊ key ��������ʱ��ʱ(�����ڵ��� 2.1.3 �汾�� Redis ���㳢�Ը��� key ������ʱ��)������ 0 ��
			*/
			bool expire(const std::string& k, int second);
			/**
			* EXPIREAT �����ú� EXPIRE ���ƣ�������Ϊ key ��������ʱ�䡣
			* ��ͬ���� EXPIREAT ������ܵ�ʱ������� UNIX ʱ���(unix timestamp)��
			*
			* ���ð汾��>= 1.2.0
			*
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �������ʱ�����óɹ������� 1 ��
			* �� key �����ڻ�û�취��������ʱ�䣬���� 0 ��
			*/
			bool expireat(const std::string& k, time_t timestamp);
			/**
			* �������� EXPIREAT �������ƣ������Ժ���Ϊ��λ���� key �Ĺ��� unix ʱ������������� EXPIREAT ����������Ϊ��λ��
			*
			* ���ð汾��>= 2.6.0
			* 
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �������ʱ�����óɹ������� 1 ��
			* �� key �����ڻ�û�취��������ʱ��ʱ������ 0 ��
			*/
			bool pexpireat(const std::string& k, time_t millisecondsTimestamp);
			/**
			* �������� EXPIRE ������������ƣ��������Ժ���Ϊ��λ���� key ������ʱ�䣬������ EXPIRE ��������������Ϊ��λ��
			*
			* ���ð汾��>= 2.6.0
			*
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �������ʱ�����óɹ������� 1 ��
			* �� key �����ڻ�û�취��������ʱ��ʱ������ 0 ��
			*/
			bool pexpire(const std::string& k, time_t milliseconds);

			/**
			* �Ƴ����� key ������ʱ�䣬����� key �ӡ���ʧ�ġ�(������ʱ�� key )ת���ɡ��־õġ�(һ����������ʱ�䡢�������ڵ� key )��
			*
			* ���ð汾��>= 2.2.0
			*
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �������ʱ�����óɹ������� 1 ��
			* �� key �����ڻ�û�취��������ʱ��ʱ������ 0 ��
			*/
			bool persist(const std::string& k);

			/**
			* �������з��ϸ���ģʽ pattern �� key
			*
			* KEYS * ƥ�����ݿ������� key ��
			* KEYS h?llo ƥ�� hello �� hallo �� hxllo �ȡ�
			* KEYS h*llo ƥ�� hllo �� heeeeello �ȡ�
			* KEYS h[ae]llo ƥ�� hello �� hallo ������ƥ�� hillo ��
			*
			* ��������� \ ����
			*
			* KEYS ���ٶȷǳ��죬����һ��������ݿ���ʹ������Ȼ��������������⣬
			* �������Ҫ��һ�����ݼ��в����ض��� key ������û����� Redis �ļ��Ͻṹ(set)�����档
			*
			* ���ð汾��>= 1.0.0
			*
			* ʱ�临�Ӷȣ�O(N)�� N Ϊ���ݿ��� key ��������
			*
			* ����ֵ��
			* ���ϸ���ģʽ�� key �б���
			*/
			reply keys(const std::string& pattern);
			reply keys();
			/**
			* �� key ԭ���Եشӵ�ǰʵ�����͵�Ŀ��ʵ����ָ�����ݿ��ϣ�һ�����ͳɹ��� key ��֤�������Ŀ��ʵ���ϣ�����ǰʵ���ϵ� key �ᱻɾ����
			* ���������һ��ԭ�Ӳ���������ִ�е�ʱ�����������Ǩ�Ƶ�����ʵ����ֱ������������������Ǩ�Ƴɹ���Ǩ��ʧ�ܣ��ȵ���ʱ��
			*
			* ������ڲ�ʵ���������ģ�
			* ���ڵ�ǰʵ���Ը��� key ִ�� DUMP ���� ��
			* �������л���Ȼ���͵�Ŀ��ʵ����Ŀ��ʵ����ʹ�� RESTORE �����ݽ��з����л������������л����õ��������ӵ����ݿ��У�
			* ��ǰʵ������Ŀ��ʵ���Ŀͻ���������ֻҪ���� RESTORE ����� OK �����ͻ���� DEL ɾ���Լ����ݿ��ϵ� key ��
			*
			* timeout �����Ժ���Ϊ��ʽ��ָ����ǰʵ����Ŀ��ʵ�����й�ͨ�������ʱ�䡣
			* ��˵����������һ��Ҫ�� timeout ��������ɣ�ֻ��˵���ݴ��͵�ʱ�䲻�ܳ������ timeout ����
			* 
			* MIGRATE ������Ҫ�ڸ�����ʱ��涨����� IO ������
			* ����ڴ�������ʱ���� IO ���󣬻��ߴﵽ�˳�ʱʱ�䣬��ô�����ִֹͣ�У�������һ������Ĵ��� IOERR ��
			*
			* �� IOERR ����ʱ�����������ֿ��ܣ�
			* key ���ܴ���������ʵ��
			* key ����ֻ�����ڵ�ǰʵ��
			*
			* Ψһ�����ܷ�����������Ƕ�ʧ key ��
			* ��ˣ����һ���ͻ���ִ�� MIGRATE ���
			* ���Ҳ������� IOERR ������ô����ͻ���ΨһҪ���ľ��Ǽ���Լ����ݿ��ϵ� key �Ƿ��Ѿ�����ȷ��ɾ����
			*
			* �������������������ô MIGRATE ��֤ key ֻ������ڵ�ǰʵ���С�
			*����Ȼ��Ŀ��ʵ���ĸ������ݿ��Ͽ����к� key ͬ���ļ���������� MIGRATE ����û�й�ϵ����
			*
			* ��ѡ�
			* COPY �����Ƴ�Դʵ���ϵ� key ��
			* REPLACE ���滻Ŀ��ʵ�����Ѵ��ڵ� key ��


			* ���ð汾��>= 2.6.0
			* 
			* ʱ�临�Ӷȣ�
			* ���������Դʵ����ʵ��ִ�� DUMP ����� DEL �����Ŀ��ʵ��ִ�� RESTORE ����鿴����������ĵ����Կ�����ϸ�ĸ��Ӷ�˵����
			* key ����������ʵ��֮�䴫��ĸ��Ӷ�Ϊ O(N) ��
			*
			* ����ֵ��
			* Ǩ�Ƴɹ�ʱ���� OK �����򷵻���Ӧ�Ĵ���
			*/
			void migrate(const std::string& host, int port, const std::string& k, int db, time_t milliseconds);
			/**
			* ����ǰ���ݿ�� key �ƶ������������ݿ� db ���С�
			* �����ǰ���ݿ�(Դ���ݿ�)�͸������ݿ�(Ŀ�����ݿ�)����ͬ���ֵĸ��� key ������ key �������ڵ�ǰ���ݿ⣬��ô MOVE û���κ�Ч����
			* ��ˣ�Ҳ����������һ���ԣ��� MOVE ������(locking)ԭ��(primitive)��
			* 
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			* ����ֵ���ƶ��ɹ����� 1 ��ʧ���򷵻� 0 ��
			*/
			bool move(const std::string& k, int db);
			/**
			* OBJECT �����������ڲ��쿴���� key �� Redis ����
			* ��ͨ�����ڳ���(debugging)�����˽�Ϊ�˽�ʡ�ռ���� key ʹ���������������
			* ����Redis�����������ʱ����Ҳ����ͨ�� OBJECT �����е���Ϣ������ key ���������(eviction policies)��
			*
			* OBJECT �����ж�������
			* OBJECT REFCOUNT <key> ���ظ��� key �����������ֵ�Ĵ�������������Ҫ���ڳ�����
			* OBJECT ENCODING <key> ���ظ��� key �������ֵ��ʹ�õ��ڲ���ʾ(representation)��
			* OBJECT IDLETIME <key> ���ظ��� key �Դ��������Ŀ�תʱ��(idle�� û�б���ȡҲû�б�д��)������Ϊ��λ��
			*
			* ��������Զ��ַ�ʽ���룺
			* �ַ������Ա�����Ϊ raw (һ���ַ���)�� int (���ַ�����ʾ64λ������Ϊ�˽�Լ�ռ�)��
			* �б����Ա�����Ϊ ziplist �� linkedlist �� ziplist ��Ϊ��Լ��С��С���б��ռ�����������ʾ��
			* ���Ͽ��Ա�����Ϊ intset ���� hashtable �� intset ��ֻ�������ֵ�С���ϵ������ʾ��
			* ��ϣ�����Ա���Ϊ zipmap ���� hashtable �� zipmap ��С��ϣ���������ʾ��
			* ���򼯺Ͽ��Ա�����Ϊ ziplist ���� skiplist ��ʽ�� ziplist ���ڱ�ʾС�����򼯺ϣ��� skiplist �����ڱ�ʾ�κδ�С�����򼯺ϡ�
			*
			* ����������ʲô�� Redis û�취��ʹ�ý�ʡ�ռ�ı���ʱ(���罫һ��ֻ�� 1 ��Ԫ�صļ�����չΪһ���� 100 ���Ԫ�صļ���)��
			* �����������(specially encoded types)���Զ�ת����ͨ������(general type)��
			*

			* ���ð汾��>= 2.2.3
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* REFCOUNT �� IDLETIME �������֡�
			* ENCODING ������Ӧ�ı������͡�
			*/
			reply object(const std::string& subcmd, const std::string& k);

			long long objectRefcount(const std::string& k);
			long long objectIdleTime(const std::string& k);
			std::string objectEncoding(const std::string& k);

			/**
			* ����Ϊ��λ�����ظ��� key ��ʣ������ʱ��(TTL, time to live)��
			*
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �� key ������ʱ������ -2 ��
			* �� key ���ڵ�û������ʣ������ʱ��ʱ������ -1 ��
			* ��������Ϊ��λ������ key ��ʣ������ʱ�䡣
			*/
			long long ttl(const std::string& k);

			/**
			* ������������� TTL ��������Ժ���Ϊ��λ���� key ��ʣ������ʱ�䣬�������� TTL ��������������Ϊ��λ��
			*
			* ���ð汾��>= 2.6.0
			* ���Ӷȣ�O(1)
			*
			* ����ֵ��
			* �� key ������ʱ������ -2 ��
			* �� key ���ڵ�û������ʣ������ʱ��ʱ������ -1 ��
			* �����Ժ���Ϊ��λ������ key ��ʣ������ʱ�䡣
			*/
			long long pttl(const std::string& k);

			/**
			* �ӵ�ǰ���ݿ����������(��ɾ��)һ�� key ��
			*
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* �����ݿⲻΪ��ʱ������һ�� key ��
			* �����ݿ�Ϊ��ʱ������ nil ��
			*/
			std::string randomkey();
			/**
			* �� key ����Ϊ newkey ��
			* �� key �� newkey ��ͬ������ key ������ʱ������һ������
			* �� newkey �Ѿ�����ʱ�� RENAME ������Ǿ�ֵ��
			* �� key ������ ���쳣
			*/
			void rename(const std::string& k, const std::string& newkey);
			/**
			*
			* ���ҽ��� newkey ������ʱ���� key ����Ϊ newkey ��
			* �� key ������ʱ������һ������
			*
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			* ����ֵ���޸ĳɹ�ʱ������ 1 ����� newkey �Ѿ����ڣ����� 0 ��
			*/
			bool renamenx(const std::string& k, const std::string& newkey);
			/**
			*
			* ���ػ򱣴�����б������ϡ����򼯺� key �о��������Ԫ�ء�
			* ����Ĭ����������Ϊ����ֵ������Ϊ˫���ȸ�������Ȼ����бȽϡ�
			*
			* ��򵥵� SORT ʹ�÷����� SORT key �� SORT key DESC ��
			* SORT key ���ؼ�ֵ��С��������Ľ����
			* SORT key DESC ���ؼ�ֵ�Ӵ�С����Ľ����
			* 
			* ���� today_cost �б������˽��յĿ����� ��ô������ SORT ���������������
			*
			* ���ð汾��>= 1.0.0
			*
			* ʱ�临�Ӷȣ�
			* O(N+M*log(M))�� N ΪҪ������б��򼯺��ڵ�Ԫ�������� M ΪҪ���ص�Ԫ��������
			* ���ֻ��ʹ�� SORT ����� GET ѡ���ȡ���ݶ�û�н�������ʱ�临�Ӷ� O(N)��
			*
			* ����ֵ��
			* û��ʹ�� STORE �����������б���ʽ����������
			* ʹ�� STORE ������������������Ԫ��������
			*/
			reply sort(const std::string& k, const std::string& pattern);

			/**
			* ���� key �������ֵ�����͡�
			*
			* ���ð汾��>= 1.0.0
			* ʱ�临�Ӷȣ�O(1)
			*
			* ����ֵ��
			* none (key������)
			* string (�ַ���)
			* list (�б�)
			* set (����)
			* zset (����)
			* hash (��ϣ��)
			*/
			reply type(const std::string& k);

			/**
			*
			* ���ڵ�����ǰ���ݿ��е����ݿ����
			*/
			reply scan(int cursor, const std::string& matchPattern = "*", int count = 10);

		private:
			client& cli_;
		};
	};
};

#if !defined(CORE_ONLY_HEADER)
#include "impl/key.hpp"
#endif

#endif