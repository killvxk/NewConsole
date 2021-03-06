#include <cstdint>

#include <intrin.h>

#include "../NewConsole/Win32Structure.hpp"
#include "../NewConsole/TargetProtocol.hpp"

typedef uint32_t (__stdcall *NtCreateFile)(void **FileHandle, int DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, 
										 PLARGE_INTEGER AllocationSize, size_t FileAttributes, size_t ShareAccess, 
										 size_t CreateDisposition, size_t CreateOptions, void *EaBuffer, size_t EaLength); 
typedef uint32_t (__stdcall *NtWriteFile)(void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
										void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key); 
typedef uint32_t (__stdcall *NtReadFile)(void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
									   void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key); 
typedef uint32_t (__stdcall *NtDeviceIoControlFile)(void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
												  size_t IoControlCode, void *InputBuffer, size_t InputBufferLength, void *OutputBuffer, size_t OutputBufferLength);
typedef uint32_t (__stdcall *NtQueryVolumeInformationFile)(void *FileHandle, PIO_STATUS_BLOCK IoStatusBlock, void **FileSystemInformation, 
														   size_t Length, int FileSystemInformationClass);
typedef uint32_t (__stdcall *NtCreateUserProcess)(void **ProcessHandle, void **ThreadHandle, size_t ProcessDesiredAccess, size_t ThreadDesiredAccess, 
												  POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, size_t ProcessFlags, 
												  size_t ThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, size_t CreateInfo, size_t AttributeList);
typedef uint32_t (__stdcall *NtDuplicateObject)(void *SourceProcessHandle, void *SourceHandle, void *TargetProcessHandle, void **TargetHandle, 
												size_t DesiredAccess, size_t HandleAttributes, size_t Options);
typedef uint32_t (__stdcall *NtClose)(void *Handle);
typedef uint32_t (__stdcall *NtConnectPort)(void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos, PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, 
											PLPC_SECTION_MEMORY ServerSharedMemory, size_t *MaximumMessageLength, void *ConnectionInfo, size_t *ConnectionInfoLength);
typedef uint32_t (__stdcall *NtSecureConnectPort)(void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos, PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, 
											void *Sid, PLPC_SECTION_MEMORY ServerSharedMemory, size_t *MaximumMessageLength, void *ConnectionInfo, size_t *ConnectionInfoLength);
typedef uint32_t (__stdcall *NtRequestWaitReplyPort)(void *PortHandle, PLPC_MESSAGE Request, PLPC_MESSAGE IncomingReply);

struct TargetData
{
	NtCreateFile originalNtCreateFile;
	NtDeviceIoControlFile originalNtDeviceIoControlFile;
	NtWriteFile originalNtWriteFile;
	NtReadFile originalNtReadFile;
	NtQueryVolumeInformationFile originalNtQueryVolumeInformationFile;
	NtCreateUserProcess originalNtCreateUserProcess;
	NtDuplicateObject originalNtDuplicateObject;
	NtClose originalNtClose;
	
	NtConnectPort originalNtConnectPort;
	NtSecureConnectPort originalNtSecureConnectPort;
	NtRequestWaitReplyPort originalNtRequestWaitReplyPort;

	bool initialized;
	void *parentProcess;

	struct ThreadData
	{
		uint32_t tid;
		void *pipe;
	};
	ThreadData threadData[1];
};

extern "C" {
__declspec(dllexport) uint32_t __stdcall HookedNtCreateFile(TargetData *targetData, void **FileHandle, int DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, 
															PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, size_t FileAttributes, size_t ShareAccess, 
															size_t CreateDisposition, size_t CreateOptions, void *EaBuffer, size_t EaLength);

__declspec(dllexport) uint32_t __stdcall HookedNtReadFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, 
														  PIO_STATUS_BLOCK IoStatusBlock, void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key);

__declspec(dllexport) uint32_t __stdcall HookedNtWriteFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, 
														   PIO_STATUS_BLOCK IoStatusBlock, void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key);

__declspec(dllexport) uint32_t __stdcall HookedNtDeviceIoControlFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, 
																	 PIO_STATUS_BLOCK IoStatusBlock,  size_t IoControlCode, void *InputBuffer, size_t InputBufferLength,
																	 void *OutputBuffer, size_t OutputBufferLength);
__declspec(dllexport) uint32_t __stdcall HookedNtQueryVolumeInformationFile(TargetData *targetData, void *FileHandle, PIO_STATUS_BLOCK IoStatusBlock, void **FileSystemInformation, 
																			size_t Length, int FileSystemInformationClass);
__declspec(dllexport) uint32_t __stdcall HookedNtCreateUserProcess(TargetData *targetData, void **ProcessHandle, void **ThreadHandle, size_t ProcessDesiredAccess, 
																   size_t ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, 
																   POBJECT_ATTRIBUTES ThreadObjectAttributes, size_t ProcessFlags,  size_t ThreadFlags, 
																   PRTL_USER_PROCESS_PARAMETERS ProcessParameters, size_t CreateInfo, size_t AttributeList);
__declspec(dllexport) uint32_t __stdcall HookedNtDuplicateObject(TargetData *targetData, void *SourceProcessHandle, void *SourceHandle, void *TargetProcessHandle, 
																 void **TargetHandle,  size_t DesiredAccess, size_t HandleAttributes, size_t Options);
__declspec(dllexport) uint32_t __stdcall HookedNtClose(TargetData *targetData, void *Handle);
__declspec(dllexport) uint32_t __stdcall HookedNtConnectPort(TargetData *targetData, void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos, 
															 PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, PLPC_SECTION_MEMORY ServerSharedMemory, 
															 size_t *MaximumMessageLength, void *ConnectionInfo, size_t *ConnectionInfoLength);
__declspec(dllexport) uint32_t __stdcall HookedNtSecureConnectPort(TargetData *targetData, void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos,
																   PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, void *Sid, PLPC_SECTION_MEMORY ServerSharedMemory, 
																   size_t *MaximumMessageLength, void *ConnectionInfo, size_t *ConnectionInfoLength);
__declspec(dllexport) uint32_t __stdcall HookedNtRequestWaitReplyPort(TargetData *targetData, void *PortHandle, PLPC_MESSAGE Request, PLPC_MESSAGE IncomingReply);
}

template<typename SrcType, typename DstType>
void copyMemory(DstType *dst, SrcType *src, size_t size)
{
	uint8_t *dst_ = reinterpret_cast<uint8_t *>(dst);
	uint8_t *src_ = reinterpret_cast<uint8_t *>(src);
	for(size_t i = 0; i < size; i ++)
		*dst_ ++ = *src_ ++;
}

void *openPipe(TargetData *targetData)
{
	void *handle = nullptr;
	IO_STATUS_BLOCK statusBlock;
	OBJECT_ATTRIBUTES attributes;
	UNICODE_STRING pipeName;
	uint8_t buf[58];

	*reinterpret_cast<uint32_t *>(buf) = 0x0044005c; //\\Device\\NamedPipe\\newconsole
	*reinterpret_cast<uint32_t *>(buf + 4) = 0x00760065;
	*reinterpret_cast<uint32_t *>(buf + 8) = 0x00630069;
	*reinterpret_cast<uint32_t *>(buf + 12) = 0x005c0065;
	*reinterpret_cast<uint32_t *>(buf + 16) = 0x0061004e;
	*reinterpret_cast<uint32_t *>(buf + 20) = 0x0065006d;
	*reinterpret_cast<uint32_t *>(buf + 24) = 0x00500064;
	*reinterpret_cast<uint32_t *>(buf + 28) = 0x00700069;
	*reinterpret_cast<uint32_t *>(buf + 32) = 0x005c0065;
	*reinterpret_cast<uint32_t *>(buf + 36) = 0x0065006e;
	*reinterpret_cast<uint32_t *>(buf + 40) = 0x00630077;
	*reinterpret_cast<uint32_t *>(buf + 44) = 0x006e006f;
	*reinterpret_cast<uint32_t *>(buf + 48) = 0x006f0073;
	*reinterpret_cast<uint32_t *>(buf + 52) = 0x0065006c;
	*reinterpret_cast<uint16_t *>(buf + 56) = 0;

	pipeName.Buffer = reinterpret_cast<wchar_t *>(buf);
	pipeName.Length = 56;
	pipeName.MaximumLength = 56;

	attributes.Length = sizeof(attributes);
	attributes.RootDirectory = nullptr;
	attributes.ObjectName = &pipeName;
	attributes.Attributes = 0;
	attributes.SecurityDescriptor = nullptr;
	attributes.SecurityQualityOfService = nullptr;

	targetData->originalNtCreateFile(&handle, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &attributes, &statusBlock, 0, 0, 0, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, 0, 0);
	return handle;
}

void *getPipeHandle(TargetData *targetData);

bool isFakeHandle(void *handle)
{
	return (reinterpret_cast<size_t>(handle) & 0xffff0000) == 0x6eff0000;
}

void sendPacketData(TargetData *targetData, void *data, size_t size)
{
	void *pipe = getPipeHandle(targetData);
	IO_STATUS_BLOCK statusBlock;
	targetData->originalNtWriteFile(pipe, 0, 0, 0, &statusBlock, data, size, 0, 0);
}

void sendPacketHeader(TargetData *targetData, uint16_t op, uint32_t length)
{
	PacketHeader header;
	header.op = op;
	header.length = length;

	sendPacketData(targetData, &header, sizeof(header));
}

void sendPacket(TargetData *targetData, uint16_t op, uint8_t *data, uint32_t size)
{
	sendPacketHeader(targetData, op, size);

	if(size)
		sendPacketData(targetData, data, size);
}

template<typename T>
void sendPacket(TargetData *targetData, uint16_t op, T *data)
{
	sendPacket(targetData, op, reinterpret_cast<uint8_t *>(data), sizeof(T));
}

void recvPacketData(TargetData *targetData, void *data, size_t size)
{
	void *pipe = getPipeHandle(targetData);
	IO_STATUS_BLOCK statusBlock;
	targetData->originalNtReadFile(pipe, 0, 0, 0, &statusBlock, data, size, 0, 0);
}

void recvPacketHeader(TargetData *targetData, PacketHeader *header)
{
	recvPacketData(targetData, header, sizeof(PacketHeader));
}

template<typename T>
uint32_t recvPacket(TargetData *targetData, T *data, size_t *length = nullptr)
{
	PacketHeader header;
	recvPacketHeader(targetData, &header);

	if(header.length)
		recvPacketData(targetData, data, header.length);

	if(length)
		*length = header.length;

	return header.op;
}

uint64_t sendInitialize(TargetData *targetData)
{
#ifdef _WIN64
	TEB *teb = reinterpret_cast<TEB *>(__readgsqword(0x30));
#elif defined(_WIN32)
	TEB *teb = reinterpret_cast<TEB *>(__readfsdword(0x18));
#endif

	InitializeRequest packet;
	packet.pid = reinterpret_cast<uint32_t>(teb->ClientId.UniqueProcess);
	packet.firstRequest = !targetData->initialized;

	sendPacket(targetData, Initialize, &packet);

	InitializeResponse response;
	recvPacket(targetData, &response);

	return response.parentProcessHandle;
}

void *getPipeHandle(TargetData *targetData)
{
#ifdef _WIN64
	TEB *teb = reinterpret_cast<TEB *>(__readgsqword(0x30));
#elif defined(_WIN32)
	TEB *teb = reinterpret_cast<TEB *>(__readfsdword(0x18));
#endif
	uint32_t tid = reinterpret_cast<uint32_t>(teb->ClientId.UniqueThread);
	TargetData::ThreadData *threadData = targetData->threadData;
	while(threadData->tid != 0)
	{
		if(threadData->tid == tid)
			break;
		threadData ++;
	}

	if(threadData->tid != tid)
	{
		threadData->tid = tid;
		threadData->pipe = openPipe(targetData);
		sendInitialize(targetData);
	}
	return threadData->pipe;
}

void initialize(TargetData *targetData)
{
	//send pid
#ifdef _WIN64
	TEB *teb = reinterpret_cast<TEB *>(__readgsqword(0x30));
#elif defined(_WIN32)
	TEB *teb = reinterpret_cast<TEB *>(__readfsdword(0x18));
#endif
	uint32_t tid = reinterpret_cast<uint32_t>(teb->ClientId.UniqueThread);
	targetData->threadData[0].tid = tid;
	targetData->threadData[0].pipe = openPipe(targetData);
	uint64_t parentProcess = sendInitialize(targetData);

	targetData->parentProcess = reinterpret_cast<void *>(parentProcess);
	targetData->initialized = true;
}

uint32_t __stdcall HookedNtCreateFile(TargetData *targetData, void **FileHandle, int DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, 
									  PLARGE_INTEGER AllocationSize, size_t FileAttributes, size_t ShareAccess, 
									  size_t CreateDisposition, size_t CreateOptions, void *EaBuffer, size_t EaLength)
{
	if(!targetData->initialized)
		initialize(targetData);
	if(ObjectAttributes && ObjectAttributes->ObjectName && FileHandle && ObjectAttributes->ObjectName->Length > 5)
	{
		if(ObjectAttributes->ObjectName->Buffer[1] != L'?' ||  //\\{Connect/Reference/~~} or \\??\\CON{IN/OUT}
		   (ObjectAttributes->ObjectName->Buffer[4] == L'C' && ObjectAttributes->ObjectName->Buffer[5] == L'O'))
		{
			HandleCreateFileRequest request;
			HandleCreateFileResponse response;
			request.fileNameLen = ObjectAttributes->ObjectName->Length;
			sendPacketHeader(targetData, HandleCreateFile, static_cast<uint32_t>(sizeof(HandleCreateFileRequest) + ObjectAttributes->ObjectName->Length + EaLength));
			sendPacketData(targetData, &request, sizeof(request));
			sendPacketData(targetData, ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length);
			if(EaLength)
				sendPacketData(targetData, EaBuffer, EaLength);

			recvPacket(targetData, &response);

			if(response.returnFake)
			{
				*FileHandle = reinterpret_cast<void *>(response.fakeHandle);
				IoStatusBlock->Information = reinterpret_cast<uint32_t *>(FILE_OPENED);
				IoStatusBlock->Status = 0;
				return 0;
			}
		}
	}
	
	return targetData->originalNtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess,
											CreateDisposition, CreateOptions, EaBuffer, EaLength);
}

uint32_t __stdcall HookedNtReadFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
									void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key)
{
	if(IoStatusBlock && Length && Buffer && isFakeHandle(FileHandle))
	{
		HandleReadFileRequest request;
		request.sizeToRead = static_cast<uint32_t>(Length);
		sendPacket(targetData, HandleReadFile, &request);

		size_t length;
		recvPacket(targetData, reinterpret_cast<uint8_t *>(Buffer), &length);
		IoStatusBlock->Information = reinterpret_cast<uint32_t *>(length);
		IoStatusBlock->Status = 0;
		return 0;
	}
	return targetData->originalNtReadFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);
}

uint32_t __stdcall HookedNtWriteFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
									 void *Buffer, size_t Length, PLARGE_INTEGER ByteOffset, size_t *Key)
{
	if(IoStatusBlock && Length && Buffer && isFakeHandle(FileHandle))
	{
		sendPacket(targetData, HandleWriteFile, reinterpret_cast<uint8_t *>(Buffer), static_cast<uint32_t>(Length));

		HandleWriteFileResponse response;
		recvPacket(targetData, &response);
		IoStatusBlock->Information = reinterpret_cast<uint32_t *>(response.writtenSize);
		IoStatusBlock->Status = 0;
		return 0;
	}
	return targetData->originalNtWriteFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);
}

uint32_t __stdcall HookedNtDeviceIoControlFile(TargetData *targetData, void *FileHandle, void *Event, void *ApcRoutine, void *ApcContext, PIO_STATUS_BLOCK IoStatusBlock, 
											   size_t IoControlCode, void *InputBuffer, size_t InputBufferLength, void *OutputBuffer, size_t OutputBufferLength)
{
	if(IoStatusBlock && isFakeHandle(FileHandle))
	{
		HandleDeviceIoControlFileRequest request;
		request.handle = reinterpret_cast<uint64_t>(FileHandle);
		request.code = static_cast<uint32_t>(IoControlCode);
		
		sendPacketHeader(targetData, HandleDeviceIoControlFile, sizeof(request) + static_cast<uint32_t>(InputBufferLength));
		sendPacketData(targetData, &request, sizeof(request));
		if(InputBufferLength)
			sendPacketData(targetData, InputBuffer, InputBufferLength);

		size_t length;
		recvPacket(targetData, reinterpret_cast<uint8_t *>(OutputBuffer), &length);
		IoStatusBlock->Information = reinterpret_cast<uint32_t *>(length);
		IoStatusBlock->Status = 0;
		return 0;
	}
	return targetData->originalNtDeviceIoControlFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, IoControlCode, InputBuffer, InputBufferLength,
													 OutputBuffer, OutputBufferLength);
}

uint32_t __stdcall HookedNtQueryVolumeInformationFile(TargetData *targetData, void *FileHandle, PIO_STATUS_BLOCK IoStatusBlock, void **FileSystemInformation, 
													  size_t Length, int FileSystemInformationClass)
{
	if(IoStatusBlock && isFakeHandle(FileHandle))
	{
		if(FileSystemInformationClass == 4) //FileFsDeviceInformation
		{
			if(Length < 8)
				return 0xc0000023; //STATUS_BUFFER_TOO_SMALL

			FILE_FS_DEVICE_INFORMATION *information = reinterpret_cast<FILE_FS_DEVICE_INFORMATION *>(FileSystemInformation);
			information->Characteristics = 0x20000; //returned value from real stdin.
			information->DeviceType = 0x50;

			IoStatusBlock->Information = reinterpret_cast<uint32_t *>(Length);
			IoStatusBlock->Status = 0;
		}
		return 0;
	}
	return targetData->originalNtQueryVolumeInformationFile(FileHandle, IoStatusBlock, FileSystemInformation, Length, FileSystemInformationClass);
}

uint32_t __stdcall HookedNtCreateUserProcess(TargetData *targetData, void **ProcessHandle, void **ThreadHandle, size_t ProcessDesiredAccess, size_t ThreadDesiredAccess, 
											 POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, size_t ProcessFlags, 
											 size_t ThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, size_t CreateInfo, size_t AttributeList)
{
	uint32_t result = targetData->originalNtCreateUserProcess(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, 
															  ThreadObjectAttributes, ProcessFlags, ThreadFlags, ProcessParameters, CreateInfo, AttributeList);
	if(!result)
	{
		void *resultHandle;
		targetData->originalNtDuplicateObject(NtCurrentProcess(), *ProcessHandle, targetData->parentProcess, &resultHandle, 0, 0, DUPLICATE_SAME_ATTRIBUTES | DUPLICATE_SAME_ACCESS);
		
		HandleCreateUserProcessRequest request;
		request.processHandle = reinterpret_cast<uint64_t>(resultHandle);
		sendPacket(targetData, HandleCreateUserProcess, &request);
		
		PacketHeader header;
		recvPacketHeader(targetData, &header); //we need to wait until host to complete patch.
	}
	return result;
}

uint32_t __stdcall HookedNtDuplicateObject(TargetData *targetData, void *SourceProcessHandle, void *SourceHandle, void *TargetProcessHandle, void **TargetHandle, 
										   size_t DesiredAccess, size_t HandleAttributes, size_t Options)
{
	if(isFakeHandle(SourceHandle) && TargetProcessHandle == NtCurrentProcess())
	{
		HandleDuplicateObjectRequest request;
		HandleDuplicateObjectResponse response;

		request.handle = reinterpret_cast<uint64_t>(SourceHandle);
		sendPacket(targetData, HandleDuplicateObject, &request);

		recvPacket(targetData, &response);
		*TargetHandle = reinterpret_cast<void *>(response.fakeHandle);
		return 0;
	}
	return targetData->originalNtDuplicateObject(SourceProcessHandle, SourceHandle, TargetProcessHandle, TargetHandle, DesiredAccess, HandleAttributes, Options);
}

uint32_t __stdcall HookedNtClose(TargetData *targetData, void *Handle)
{
	if(isFakeHandle(Handle))
		return 0;
	return targetData->originalNtClose(Handle);
}

void onConnectPort(TargetData *targetData, void *serverBase, void *clientBase)
{
	LPCConnectRequest request;
	request.clientBase = reinterpret_cast<size_t>(clientBase);
	request.serverBase = reinterpret_cast<size_t>(serverBase);

	sendPacket(targetData, HandleLPCConnect, &request);

	PacketHeader header;
	recvPacketHeader(targetData, &header);
}

uint32_t __stdcall HookedNtConnectPort(TargetData *targetData, void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos, 
									   PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, PLPC_SECTION_MEMORY ServerSharedMemory, size_t *MaximumMessageLength, 
									   void *ConnectionInfo, size_t *ConnectionInfoLength)
{
	if(!targetData->initialized)
		initialize(targetData);

	//\\RPC Control\\console-0x%p-lpc-handle or \\RPC Control\\ConsoleLPC-0x%p-%ws
	if(ServerPortName && ServerPortName->Buffer && ServerPortName->Buffer[1] == L'R' && ServerPortName->Buffer[14] == L'o' && ServerPortName->Buffer[15] == L'n' && ClientSharedMemory)
	{
		ServerSharedMemory->Length = ClientSharedMemory->Length;
		ServerSharedMemory->ViewBase = ClientSharedMemory->ViewBase;
		ServerSharedMemory->ViewSize = ClientSharedMemory->ViewSize;
		*reinterpret_cast<uint32_t *>(ClientPortHandle) = 1;
		onConnectPort(targetData, ClientSharedMemory->OtherSideViewBase, ClientSharedMemory->ViewBase);
		return 0;
	}
	return targetData->originalNtConnectPort(ClientPortHandle, ServerPortName, SecurityQos, ClientSharedMemory, ServerSharedMemory, MaximumMessageLength, 
													 ConnectionInfo, ConnectionInfoLength);
}

uint32_t __stdcall HookedNtSecureConnectPort(TargetData *targetData, void **ClientPortHandle, PUNICODE_STRING ServerPortName, size_t SecurityQos,
											 PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, void *Sid, PLPC_SECTION_MEMORY ServerSharedMemory, 
											 size_t *MaximumMessageLength, void *ConnectionInfo, size_t *ConnectionInfoLength)
{
	if(!targetData->initialized)
		initialize(targetData);

	//\\RPC Control\\console-0x%p-lpc-handle or \\RPC Control\\ConsoleLPC-0x%p-%ws
	if(ServerPortName && ServerPortName->Buffer && ServerPortName->Buffer[1] == L'R' && ServerPortName->Buffer[14] == L'o' && ServerPortName->Buffer[15] == L'n' && ClientSharedMemory)
	{
		ServerSharedMemory->Length = ClientSharedMemory->Length;
		ServerSharedMemory->ViewBase = ClientSharedMemory->ViewBase;
		ServerSharedMemory->ViewSize = ClientSharedMemory->ViewSize;
		*reinterpret_cast<uint32_t *>(ClientPortHandle) = 1;
		onConnectPort(targetData, ClientSharedMemory->OtherSideViewBase, ClientSharedMemory->ViewBase);
		return 0;
	}
	return targetData->originalNtSecureConnectPort(ClientPortHandle, ServerPortName, SecurityQos, ClientSharedMemory, Sid, ServerSharedMemory, 
														   MaximumMessageLength, ConnectionInfo, ConnectionInfoLength);
}

uint32_t __stdcall HookedNtRequestWaitReplyPort(TargetData *targetData, void *PortHandle, PLPC_MESSAGE Request, PLPC_MESSAGE IncomingReply)
{
	HandleLPCMessageRequest request;
	request.requestPointer = reinterpret_cast<uint64_t>(Request);
	sendPacketHeader(targetData, HandleLPCMessage, Request->Length + sizeof(request));
	sendPacketData(targetData, &request, sizeof(request));
	sendPacketData(targetData, Request, Request->Length);

	PacketHeader header;
	HandleLPCMessageResponse response;
	recvPacketHeader(targetData, &header);
	recvPacketData(targetData, &response, sizeof(HandleLPCMessageResponse));

	if(response.callOriginal)
		return targetData->originalNtRequestWaitReplyPort(PortHandle, Request, IncomingReply);
	else
	{
		recvPacketData(targetData, IncomingReply, header.length - sizeof(HandleLPCMessageResponse));
		return 0;
	}
}
