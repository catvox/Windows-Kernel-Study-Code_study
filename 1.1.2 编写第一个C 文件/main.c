#include <ntddk.h>

// �ṩһ��Unload����ֻ��Ϊ������������ܶ�̬ж�أ��������
VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
	// ����ʵ��������ʲô��������ֻ��ӡһ�仰
	DbgPrint("first��Our driver\r\n");
}

// DriverEntry����ں������൱��mian
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
#if DBG
	// ����� int3 ��һ��������ԣ�ִ�е�����ʱ������������������ֹ�������һ���ϵ㡣
	// ���������Ĵ���������ǵ���״ִ̬�оͻ�ֱ�����������Լ��Ϻ� DBG ���ԣ���ֻ֤�е��԰汾��
	// �������ű�����
	_asm int 3
#endif
	pDriver;
	pReg_path;
	// �������ں�ģ�����ڣ�����������д��������Ҫ�Ķ���
	DbgPrint("first: Hello, my salary!");
	PAGED_CODE();
	// ����һ��ж�غ�����������������˳�
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}