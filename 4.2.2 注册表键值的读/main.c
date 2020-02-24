#include <ntddk.h>
#define MEM_TAG1 'ddd'
#define MEM_TAG2 'eee'

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS ReadRegister(PUNICODE_STRING pKeyPath, PUNICODE_STRING pKeyName)
{
	NTSTATUS status;
	HANDLE MyKeyHandle = NULL;
	OBJECT_ATTRIBUTES MyObjAttr = { 0 };
	// ������̽��С��KeyInfor
	KEY_VALUE_PARTIAL_INFORMATION KeyInfor;
	// ���ʵ���õ���KeyInforָ�롣�ڴ�����ڶ���
	PKEY_VALUE_PARTIAL_INFORMATION AcKeyInfor;
	ULONG uAcLength;

	// ��ʼ��OBJECT_ATTRIBUTES
	InitializeObjectAttributes(
		&MyObjAttr,
		pKeyPath,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);
	status = ZwOpenKey(&MyKeyHandle, KEY_READ, &MyObjAttr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��ע���ʧ��"));
		return status;
	}
	status = ZwQueryValueKey(
		MyKeyHandle,
		pKeyName,
		KeyValuePartialInformation,
		&KeyInfor,
		sizeof(KEY_VALUE_PARTIAL_INFORMATION),
		&uAcLength);
	if (!NT_SUCCESS(status) &&
		status != STATUS_BUFFER_OVERFLOW &&
		status != STATUS_BUFFER_TOO_SMALL)
	{
		KdPrint(("��ȡע����ֵʧ��"));
		return status;
	}
	// ���û��ʧ�ܣ���ô�����㹻�Ŀռ䣬�ٴζ�ȡ��
	AcKeyInfor = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, uAcLength, MEM_TAG1);
	if (AcKeyInfor == NULL)
	{
		KdPrint(("����ʧ��"));
		status = STATUS_INSUFFICIENT_RESOURCES;
		return status;
	}
	status = ZwQueryValueKey(
		MyKeyHandle,
		pKeyName,
		KeyValuePartialInformation,
		AcKeyInfor,
		uAcLength,
		&uAcLength);
	// ����λ�ã����statusΪSTATUS_SUCCESS����Ҫ��ȡ�������Ѿ�
	// ��AcKeyInfor->Data������
	PUCHAR Buffer = (PUCHAR)ExAllocatePoolWithTag(NonPagedPool, AcKeyInfor->DataLength, MEM_TAG2);
	RtlCopyMemory(Buffer, AcKeyInfor->Data, AcKeyInfor->DataLength);
	UNICODE_STRING Buff;
	Buff.Buffer = (PWCHAR)Buffer;
	Buff.Length = Buff.MaximumLength = (USHORT)AcKeyInfor->DataLength;
	KdPrint(("%wZ", &Buff));
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	NTSTATUS status;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"SystemRoot");
	status = ReadRegister(&KeyPath, &KeyName);
	pDriver->DriverUnload = DriverUnload;
	return status;
}