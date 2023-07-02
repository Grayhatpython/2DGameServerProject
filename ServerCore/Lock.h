#pragma once

class Lock
{
    enum : uint32
    {
        ACQUIRE_TIMEOUT_TICK = 10000,
        MAX_SPIN_COUNT = 1024,
        WRITE_THREAD_MASK = 0xFFFF'0000,
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };

public:
    Lock();
    ~Lock();

public:
    void WriteLock();
    void WriteUnlock();
    void ReadLock();
    void ReadUnlock();

public:
    void SetName(const std::string& name) { _name = name; }

private:
	std::atomic<uint32> _lockFlag = EMPTY_FLAG;;
    std::string         _name; 
	uint16				_writeCount = 0;
    uint32              _lockId = 0;
};

class WriteLockGuard
{
public:
    WriteLockGuard(Lock& lock, const std::string& name) : _lock(lock) 
    { 
        _lock.SetName(name);  
        _lock.WriteLock(); 
    }
    ~WriteLockGuard() { _lock.WriteUnlock(); }

private:
    Lock& _lock;
};

class ReadLockGuard
{
public:
    ReadLockGuard(Lock& lock, const std::string& name) : _lock(lock) 
    { 
        _lock.SetName(name);  
        _lock.ReadLock(); 
    }
    ~ReadLockGuard() { _lock.ReadUnlock(); }

private:
    Lock& _lock;
};

