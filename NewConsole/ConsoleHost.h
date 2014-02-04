#pragma once

#include <cstdint>
#include <string>
#include <functional>
#include <list>

class ConsoleEventListener;
class ConsoleHostConnection;
class ConsoleHost
{
	friend class ConsoleHostServer;
private:
	int inputMode_;
	int outputMode_;
	void *childProcess_;
	uint32_t childProcessId_;
	ConsoleHostConnection *connection_;
	ConsoleEventListener *listener_;
	std::string inputBuffer_;
	int lastHandleId_;
	std::list<uint32_t> inputHandles_;
	std::list<uint32_t> outputHandles_;

	std::list<std::tuple<size_t, std::function<void (const uint8_t *, size_t)>, bool>> queuedReadOperations_;

	uint32_t newFakeHandle();
	void cleanup();
	void queueReadOperation(size_t size, const std::function<void (const uint8_t *, size_t)> &completion, bool isWidechar);
	void checkQueuedRead();
	void handleWrite(uint8_t *buffer, size_t bufferSize);
	bool isInputHandle(uint32_t handle);
	bool isOutputHandle(uint32_t handle);
public:
	ConsoleHost(const std::wstring &cmdline, ConsoleEventListener *listener);
	~ConsoleHost();

	void write(const std::string &buffer);
	void handlePacket(uint16_t op, uint32_t size, uint8_t *data);
	void handleDisconnected();
	void setConnection(ConsoleHostConnection *connection);
};