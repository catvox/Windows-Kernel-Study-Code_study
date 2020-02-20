#include <ntddk.h>
#define MEM_TAG 'ddd'
LIST_ENTRY MyListHead;	// ����ͷ
KSPIN_LOCK MyListLock;	// �������

// ����ͷ��ʼ����һ���ڳ�����ڴ�����һ��
void MyFileInforInit()
{
	InitializeListHead(&MyListHead);
	KeInitializeSpinLock(&MyListHead);
}

VOID DriverUnload(PDRIVER_OBJECT pDriver)
{
	pDriver;
}

// ����ڵ㣬���汣��һ���ļ�����һ���ļ�������Ϣ
typedef struct
{
	LIST_ENTRY ListEntry;
	PFILE_OBJECT FileObject;
	PUNICODE_STRING FileName;
	LARGE_INTEGER FileLength;
}MYFILEINFO, * PMYFILEINFO;

// ׷��һ����Ϣ��Ҳ��������һ�������㡣��ע�⣬FileName����������
// �ڴ���ʹ���߹�������������������
NTSTATUS MyFileInforAppendNode(
	PFILE_OBJECT FileObject,
	PUNICODE_STRING FileName,
	LARGE_INTEGER FileLength
)
{
	PMYFILEINFO MyFileInfor = (PMYFILEINFO)ExAllocatePoolWithTag(PagedPool, sizeof(MYFILEINFO), MEM_TAG);
	if (MyFileInfor == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;
	// ��д���ݳ�Ա
	MyFileInfor->FileObject = FileObject;
	MyFileInfor->FileName = FileName;
	MyFileInfor->FileLength = FileLength;

	// ʹ����������Ĳ��������������ͨ�Ĳ���
	ExInterlockedInsertHeadList(&MyListHead, (PLIST_ENTRY)&MyFileInfor, &MyListLock);
	// ʹ�����������ɾ������
	MyFileInfor = ExInterlockedRemoveHeadList(&MyListHead, &MyListLock);
	return STATUS_SUCCESS;
}
