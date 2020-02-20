#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

KSPIN_LOCK MySpinLock;

VOID MySafeFunction()
{
	KIRQL irql;
	// ������������irql���ڱ��浱ǰ���ж�����
	KeAcquireSpinLock(&MySpinLock, &irql);
	// To do something ...
	// �ͷ���������irql���浱ǰ���ж����󼶱�ע�⣬����������Ǹ�������������һ��������ָ�������
	KeReleaseSpinLock(&MySpinLock, irql);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// KeInitializeSpinLock ����û�з���ֵ��
	KeInitializeSpinLock(&MySpinLock);
	MySafeFunction();
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}