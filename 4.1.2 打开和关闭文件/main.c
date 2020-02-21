#include <ntddk.h>

VOID DriveUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	// Ҫ���ص��ļ����
	HANDLE FileHandle = NULL;
	// ����ֵ
	NTSTATUS status;
	// ����IoStatusBlock����ṹ
	IO_STATUS_BLOCK io_status;
	// ���ȳ�ʼ�������ļ�·����OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING uFileName = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	InitializeObjectAttributes(
		&ObjectAttributes,
		&uFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	// ��FILE_OPEN_IF��ʽ���ļ�
	status = ZwCreateFile(
		&FileHandle,
		GENERIC_READ | GENERIC_WRITE,
		&ObjectAttributes,
		&io_status,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);
	ZwClose(FileHandle);
	pDriver->DriverUnload = DriveUnload;
	return status;
}