#ifndef __OS_AVR8__
#define __OS_AVR8__

#include <medusa/namespace.hpp>
#include <medusa/os.hpp>

#if defined(_WIN32) || defined(WIN32)
#ifdef os_avr8_EXPORTS
#define OS_AVR8_EXPORT __declspec(dllexport)
#else
#define OS_AVR8_EXPORT __declspec(dllimport)
#endif
#else
#define OS_AVR8_EXPORT
#endif

MEDUSA_NAMESPACE_USE


class Avr8OperatingSystem : public OperatingSystem {
public:
    virtual std::string GetName(void) const;

    virtual bool InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const;
    virtual bool InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const;
    virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const;

    virtual bool AnalyzeFunction(Document& rDoc, Address const& rAddress);

    virtual bool ProvideDetails(Document& rDoc) const;
    virtual bool GetValueDetail(Id ValueId, ValueDetail& rValDtl) const;
    virtual bool GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const;
    virtual bool GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const;
};

extern "C" OS_AVR8_EXPORT OperatingSystem* GetOperatingSystem(void);

#endif // !__OS_AVR8__
