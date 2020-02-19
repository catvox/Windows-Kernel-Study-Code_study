#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	DbgPrint("Unload\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	// ��һ�����ӣ���һ��UNICODEƴ��һ���ַ���
	UNICODE_STRING dst;	// Ŀ���ַ���
	WCHAR DstBuff[256];	// ���建����
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// ��Ŀ���ַ�����ʼ��Ϊӵ�л���������Ϊ256��UNICODE_STRING�մ�
	RtlInitEmptyUnicodeString(&dst, DstBuff, 256 * sizeof(WCHAR));
	RtlCopyUnicodeString(&dst, &src);	// �ַ�������
	// UNICODE_STRINGƴ����һ���ַ���ʹ��RtlAppendUnicodeToString
	RtlAppendUnicodeToString(&dst, L"my second string");
	DbgPrint("%wZ\n", &dst);
	// �ڶ������ӣ���һ��UNICODE_STRING������һ��UNICODE_STRING
	UNICODE_STRING str1;
	UNICODE_STRING str2;
	WCHAR strBuff1[256];	
	WCHAR strBuff2[256];
	RtlInitEmptyUnicodeString(&str1, strBuff1, 256 * sizeof(WCHAR));
	RtlInitEmptyUnicodeString(&str2, strBuff2, 256 * sizeof(WCHAR));
	UNICODE_STRING src1 = RTL_CONSTANT_STRING(L"First");
	UNICODE_STRING src2 = RTL_CONSTANT_STRING(L"Second");
	RtlCopyUnicodeString(&str1, &src1);
	RtlCopyUnicodeString(&str2, &src2);
	//UNICODE_STRINGƴ��UNICODE_STRINGʹ��RtlAppendUnicodeStringToString
	RtlAppendUnicodeStringToString(&str1, &str2);
	DbgPrint("%wZ\n", &str1);
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}