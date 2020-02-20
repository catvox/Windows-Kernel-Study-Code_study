#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// ����һ���ڴ������
#define MEM_TAG 'MyTt'
	// Ŀ���ַ���������������Ҫ����ռ�
	UNICODE_STRING dst = { 0 };
	UNICODE_STRING src = RTL_CONSTANT_STRING(L"My source string!");
	// ����Դ�ַ����ĳ��ȣ�����ռ��Ŀ���ַ���
	dst.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, src.Length, MEM_TAG);
	if (dst.Buffer == NULL)
	{
		// ������
		status = STATUS_INSUFFICIENT_RESOURCES;
	}
	else
	{
		dst.Length = dst.MaximumLength = src.Length;
		RtlCopyUnicodeString(&dst, &src);
		KdPrint(("%wZ", &dst));
		status = STATUS_SUCCESS;
	}
	pDriver->DriverUnload = DriverUnload;
	return status;
}