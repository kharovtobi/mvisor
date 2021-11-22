#ifndef MVISOR_MACHINE_H
#define MVISOR_MACHINE_H

#define PAGE_SIZE 4096

#include <thread>
#include <vector>
#include <signal.h>
#include "vcpu.h"
#include "io_thread.h"
#include "memory_manager.h"
#include "device_manager.h"

class Machine {
 public:
  Machine(int vcpus, uint64_t ram_size);
  ~Machine();

  int Run();
  void Quit();
  bool IsValid() { return valid_; }
  void Interrupt(uint32_t irq, uint32_t level);
  void Reset();

  inline DeviceManager* device_manager() { return device_manager_; }
  inline MemoryManager* memory_manager() { return memory_manager_; }
  int num_vcpus() { return num_vcpus_; }

 private:
  friend class Vcpu;
  friend class MemoryManager;

  void InitializeKvm();
  void CreateArchRelated();
  void CreateVcpu();
  Device* CreateQ35();
  void LoadBiosFile(const char* path);

  bool valid_ = true;
  int kvm_fd_ = -1;
  int kvm_vcpu_mmap_size_ = 0;
  int vm_fd_ = -1;
  
  int num_vcpus_ = 0;
  std::vector<Vcpu*> vcpus_;
  MemoryManager* memory_manager_;
  DeviceManager* device_manager_;
  IoThread* io_thread_;

  uint64_t ram_size_;
  size_t bios_size_;
  void* bios_data_ = nullptr;
  void* bios_backup_ = nullptr;
};

#endif // MVISOR_MACHINE_H
