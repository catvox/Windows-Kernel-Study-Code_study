#include <ntddk.h>
#include <ntstrsafe.h>

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegPath)
{
	pDriver;
	pRegPath;
	WCHAR DstBuf[512] = { 0 };
	UNICODE_STRING dst;
	NTSTATUS status;
	UNICODE_STRING FilePath = RTL_CONSTANT_STRING(L"\\??\\c:\\winddk\\7600.16385.1\\inc\\cifs.h");
	USHORT FileSize = 1024;
	// �ַ�����ʼ��Ϊ�մ�������������Ϊ*sizeof(WCHAR)
	RtlInitEmptyUnicodeString(&dst, DstBuf, 512 * sizeof(WCHAR));
	// ����RtlStringCbPrintfW�����д�ӡ
	// ���������sprintfwһ�������������ն���������Ǵ�ӡ������ַ�����
	status = RtlStringCbPrintfW(dst.Buffer, 512 * sizeof(WCHAR), L"file path = %wZ file size = %d \r\n", &FilePath, FileSize);
#if DBG
	_asm int 3
#endif
	// �������wcslenû���⣬������ΪRtlStringCbPrintfW��ӡ��
	// �ַ������Կս�����
	dst.Length = (USHORT)wcslen(dst.Buffer) * sizeof(WCHAR);
	DbgPrint("%wZ", &dst);
	pDriver->DriverUnload = DriverUnload;
	return status;
}