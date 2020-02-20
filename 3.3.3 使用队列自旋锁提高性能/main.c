#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

KSPIN_LOCK MyQueueSpinLock;
KLOCK_QUEUE_HANDLE MyLockQueueHandle;

VOID MySafeFunction()
{
	// �������������,������������ʹ��������һ�� KLOCK_QUEUE_HANDLE ���ݽṹ
	// ������ݽṹΨһ�ر�ʾһ������������
	KeAcquireInStackQueuedSpinLock(&MyQueueSpinLock, &MyLockQueueHandle);
	// To do something ...
	// �ͷŶ�����������
	KeReleaseInStackQueuedSpinLock(&MyLockQueueHandle);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	NTSTATUS status;
	// KeInitializeSpinLock ����û�з���ֵ��
	KeInitializeSpinLock(&MyQueueSpinLock);
	MySafeFunction();
	pDriver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}