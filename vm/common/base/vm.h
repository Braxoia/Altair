#ifndef ALTAIR_VM_H_INCLUDED
#define ALTAIR_VM_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALTAIR_DEFINE_TYPE(name) typedef struct name##_T* name

ALTAIR_DEFINE_TYPE(ArVirtualMachine);
ALTAIR_DEFINE_TYPE(ArProcessor);
ALTAIR_DEFINE_TYPE(ArPhysicalMemory);

typedef enum ArResult
{
    AR_END_OF_CODE = 1,
    AR_SUCCESS = 0,
    AR_ERROR_ILLEGAL_INSTRUCTION = -1,
    AR_ERROR_INVALID_CODE = -2,
    AR_ERROR_HOST_OUT_OF_MEMORY = -256,
} ArResult;

typedef enum ArStructureType
{
    AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO = 0,
    AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO = 1,
    AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO = 2,
} ArStructureType;

typedef struct ArVirtualMachineCreateInfo
{
    ArStructureType sType; //< The type of this structure
    void* pNext;           //< A pointer to the next structure
} ArVirtualMachineCreateInfo;

typedef struct ArProcessorCreateInfo
{
    ArStructureType sType;     //< The type of this structure
    void* pNext;               //< A pointer to the next structure
    const uint32_t* pBootCode; //< The boot code as an array of 32-bits unsigned integers
    uint32_t bootCodeSize;     //< The number of op-codes in pBootCode array
} ArProcessorCreateInfo;

typedef struct ArPhysicalMemoryCreateInfo
{
    ArStructureType sType; //< The type of this structure
    void* pNext;           //< A pointer to the next structure
    void* pMemory;         //< A pointer to the memory beginning
    uint64_t size;         //< The number of bytes of the memory
} ArPhysicalMemoryCreateInfo;

/** \brief Creates a new virtual machine

    \param pVirtualMachine A pointer to a ArVirtualMachine handle
    \param pInfo A pointer on a valid ArVirtualMachineCreateInfo instance

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo);

/** \brief Creates a new processor within a virtual machine

    \param virtualMachine A ArVirtualMachine handle
    \param pInfo A pointer on a valid ArProcessorCreateInfo instance
    \param pProcessor A pointer to a ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_INVALID_CODE if the code is empty, or its size is not a multiple of 4
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arCreateProcessor(ArVirtualMachine virtualMachine, const ArProcessorCreateInfo* pInfo, ArProcessor* pProcessor);

/** \brief Creates a new physical memory device within a virtual machine

    \param virtualMachine A ArVirtualMachine handle
    \param pInfo A pointer on a valid ArPhysicalMemoryCreateInfo instance
    \param pProcessor A pointer to a ArPhysicalMemory handle

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arCreatePhysicalMemory(ArVirtualMachine virtualMachine, const ArPhysicalMemoryCreateInfo* pInfo, ArPhysicalMemory* pMemory);

/** \brief Decode the next instructions and increment the program counter

    \param processor A ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_END_OF_CODE if the processor reached the end of its code
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arDecodeInstruction(ArProcessor processor);

/** \brief Run the instruction on the processor, and update its status

    \param processor A ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arExecuteInstruction(ArProcessor processor);

/** \brief Run the instruction on the processor, and update its status

    \param processor A ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
ArResult arExecuteDirectMemoryAccess(ArProcessor processor);

/** \brief Destroy a virtual machine

    All subobjects must have been freed

    \param virtualMachine A ArVirtualMachine handle
*/
void arDestroyVirtualMachine(ArVirtualMachine virtualMachine);

/** \brief Destroy a processor within the virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine A ArVirtualMachine handle
    \param processor A ArProcessor handle
*/
void arDestroyProcessor(ArVirtualMachine virtualMachine, ArProcessor processor);

/** \brief Destroy a physical memory device within a virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine A ArVirtualMachine handle
    \param processor A ArPhysicalMemory handle
*/
void arDestroyPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory);

#undef ALTAIR_DEFINE_TYPE


#ifdef __cplusplus
}
#endif

#endif
