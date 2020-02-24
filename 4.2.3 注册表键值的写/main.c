#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS WriteRegister(PUNICODE_STRING pKeyPath, PUNICODE_STRING pName, PWCHAR pValue)
{
	NTSTATUS status;
	HANDLE MyKeyHandle = NULL;
	OBJECT_ATTRIBUTES MyObjAttr = { 0 };
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
	// д�����ݡ����ݳ���֮����Ҫ���ַ������ȼ���1����Ϊ�˰����һ���ս�����
	// д�롣���߲�ȷ�������д��ս������᲻���д�����Ȥ���Բ���һ�¡�
	// ��ʵ�ʲ�����һ�£����������
	status = ZwSetValueKey(MyKeyHandle, pName, 0, REG_SZ, pValue, (wcslen(pValue) + 1) * sizeof(WCHAR));
	if (!NT_SUCCESS(status))
	{
		KdPrint(("д��ע���ʧ��"));
		return status;
	}
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	NTSTATUS status;
	UNICODE_STRING KeyPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	UNICODE_STRING Name = RTL_CONSTANT_STRING(L"Test");
	PWCHAR pValue = { L"My Test Value" };
	status = WriteRegister(&KeyPath, &Name, pValue);
	pDriver->DriverUnload = DriverUnload;
	return status;
}