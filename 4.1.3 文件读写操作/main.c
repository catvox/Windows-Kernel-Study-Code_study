#include <ntddk.h>
#define MEM_TAG 'MyTt'

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

NTSTATUS MyCopyFile(PUNICODE_STRING TargetPath, PUNICODE_STRING SourcePath)
{
	NTSTATUS status;
	// Ŀ������Դ���
	HANDLE TargetHandle, SourceHandle = NULL;
	// ����IoStatusBlock����ṹ
	IO_STATUS_BLOCK TarIoStatus, SouIoStatus;
	// ���ȳ�ʼ�������ļ�·����OBJECT_ATTRIBUTES
	OBJECT_ATTRIBUTES TarObjectAttributes, SouObjectAttributes;
	InitializeObjectAttributes(
		&SouObjectAttributes,
		SourcePath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	InitializeObjectAttributes(
		&TarObjectAttributes,
		TargetPath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
	);
	// ��FILE_OPEN��ʽ��Դ�ļ�������ļ������ڣ��򷵻ظ���ʧ��
	status = ZwCreateFile(
		&SourceHandle,
		GENERIC_READ,
		&SouObjectAttributes,
		&SouIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	// �����ʧ�ܵĻ����ͷ�����
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��Դ�ļ�ʧ��\n"));
		return status;
	}
	// ��FILE_CREATE��ʽ��Դ�ļ���
	status = ZwCreateFile(
		&TargetHandle,
		GENERIC_WRITE,
		&TarObjectAttributes,
		&TarIoStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_CREATE,
		FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0
	);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�����ļ�ʧ��\n"));
		return status;
	}
	// ��������ȡ�ļ����ݵ�Buffer
	// ����һ��4kb�Ŀռ�
	ULONG uLength = 4 * 1024;
	LARGE_INTEGER offset = { 0 };
	PVOID Buffer = ExAllocatePoolWithTag(NonPagedPool, 4 * 1024, MEM_TAG);
	do
	{
		if (Buffer == NULL)
			break;
		while (1)
		{
			RtlZeroMemory(Buffer, uLength);
			status = ZwReadFile(
				SourceHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
			{
				// ���״̬ΪSTATUS_END_OF_FILE�� 
				// ��˵���ļ��Ŀ����Ѿ��ɹ��ؽ����ˡ�
				if (status == STATUS_END_OF_FILE)
					status = STATUS_SUCCESS;
				break;
			}
			// ���ʵ�ʶ�ȡ���ĳ���
			uLength = TarIoStatus.Information;
			// ���ڶ�ȡ�����ݡ������ĳ���Ϊlength����ôд��ĳ���ҲӦ����length��
			// д�����ɹ������ʧ�ܣ��򷵻ش���
			status = ZwWriteFile(
				TargetHandle,
				NULL,
				NULL,
				NULL,
				&TarIoStatus,
				Buffer,
				uLength,
				&offset,
				NULL
			);
			if (!NT_SUCCESS(status))
				break;

			// offset �ƶ���Ȼ�������ֱ������
			// STATUS_END_OF_FILE ��ʱ��ʼ���
			offset.QuadPart += uLength;
		}
	} while (0);
	// ���˳�֮ǰ���ͷ���Դ���ر����еľ����
	if (TargetHandle != NULL)
		ZwClose(TargetHandle);
	if (SourceHandle != NULL)
		ZwClose(SourceHandle);
	if (Buffer != NULL)
		ExFreePool(Buffer);
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg_path)
{
	pDriver;
	pReg_path;
	// ����ֵ
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING TargetPath = RTL_CONSTANT_STRING(L"\\??\\C:\\2.txt");
	UNICODE_STRING SourcePath = RTL_CONSTANT_STRING(L"\\??\\C:\\1.txt");
	status = MyCopyFile(&TargetPath, &SourcePath);
	pDriver->DriverUnload = DriverUnload;
	return status;
}