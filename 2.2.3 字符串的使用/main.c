#include <ntddk.h>

// �ṩһ��Unload����ֻ��Ϊ������������ܶ�̬ж�أ��������
VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	// ����ʵ��������ʲô��������ֻ��ӡһ�仰
	DbgPrint("first��Our driver\r\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
#if DBG
	_asm int 3
#endif
	pDriver;
	pReg_path;
	// �������ں�ģ�����ڣ�����������д��������Ҫ�Ķ���
	DbgPrint("first: Hello, my salary!");

	// ����ʹ��UNICODE_STRING�������ַ������Ҵ�ӡ
	UNICODE_STRING str = RTL_CONSTANT_STRING(L"first: Hello, my salary! ");
	DbgPrint("%wZ", &str);
	// ����һ��ж�غ�����������������˳�
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}