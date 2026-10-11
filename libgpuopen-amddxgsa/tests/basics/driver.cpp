#include <cstring>
#include <stdio.h>

#include <AsicReg/devices.h>

// The shader compile extension only targets Windows (it includes
// <windows.h>).
//
#ifdef _WIN32
#  include <GSA/AmdDxGsaCompile.h>
#endif

#undef NDEBUG
#include <cassert>

#ifdef _WIN32
#ifndef LOAD_LIBRARY_SEARCH_SYSTEM32
#  define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800
#endif

#if defined (_WIN64)
static const char* const system_dlls[] = {"amdxx64.dll", "atidxx64.dll"};
#else
static const char* const system_dlls[] = {"amdxx32.dll", "atidxx32.dll"};
#endif
#endif

int main ()
{
  using namespace std;

  // Chip families and revisions.
  //
  {
    assert (ATI_VENDOR_ID == 0x1002);

    assert (FAMILY_NORTHERNISLAND == FAMILY_NI);
    assert (FAMILY_SI < FAMILY_CI && FAMILY_CI < FAMILY_VI);
    assert (FAMILY_AI < FAMILY_NV && FAMILY_NV < FAMILY_NV3);

    assert (SI_TAHITI_P_A0 == SI_TAHITI_P_A11);
    assert (SI_UNKNOWN == 0xFF);
    assert (VI_ELLESMERE_P_A1 == 81);
    assert (NV_NAVI21_P_A0 == 40);
    assert (AI_VEGA20_P_A0 == 40);
  }

#ifdef _WIN32
  // Shader compile input, output, and statistics types.
  //
  {
    AmdDxGsaCompileOption o;
    o.setting = AmdDxGsaEnableShaderIntrinsics;
    o.value = 1;
    assert (AmdDxGsaEnableShaderIntrinsics < AmdDxGsaCompileOptionLast);

    AmdDxGsaCompileShaderInput in;
    memset (&in, 0, sizeof (in));
    in.chipFamily = FAMILY_NV;
    in.chipRevision = NV_NAVI21_P_A0;
    in.pCompileOptions = &o;
    in.numCompileOptions = 1;
    in.inputType = GsaInputIlText;
    assert (in.pShaderByteCode == nullptr && in.byteCodeLength == 0);

    AmdDxGsaCompileShaderOutput out;
    memset (&out, 0, sizeof (out));
    out.size = sizeof (out);
    assert (out.pShaderBinary == nullptr && out.shaderBinarySize == 0);

    AmdDxGsaCompileStats s;
    memset (&s, 0, sizeof (s));
    assert (s.numSgprsUsed == 0 && s.numVgprsUsed == 0);
  }

  // Function pointer types for the entry points exported by the AMD
  // Direct3D 11 driver (resolved at runtime with GetProcAddress()).
  //
  {
    PfnAmdDxGsaCompileShader c (nullptr);
    PfnAmdDxGsaFreeCompiledShader f (nullptr);
    assert (c == nullptr && f == nullptr);
  }

  // Probe a system-installed driver in System32 only.
  //
  HMODULE mod = NULL;
  const char* loaded = NULL;
  for (size_t i = 0; i < sizeof (system_dlls) / sizeof (system_dlls[0]); ++i)
  {
    mod = LoadLibraryExA (system_dlls[i], NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (mod != NULL)
    {
      loaded = system_dlls[i];
      break;
    }
  }

  if (mod == NULL)
  {
    printf ("libgpuopen-amddxgsa - no system Direct3D 11 driver\n");
    return 0;
  }

  printf ("libgpuopen-amddxgsa - loaded %s\n", loaded);

  PfnAmdDxGsaCompileShader compile =
    (PfnAmdDxGsaCompileShader) GetProcAddress (mod, "AmdDxGsaCompileShader");
  PfnAmdDxGsaFreeCompiledShader free_shader =
    (PfnAmdDxGsaFreeCompiledShader) GetProcAddress (mod, "AmdDxGsaFreeCompiledShader");

  if (compile == NULL || free_shader == NULL)
  {
    printf ("libgpuopen-amddxgsa - missing GSA exports\n");
    FreeLibrary (mod);
    return 0;
  }

  AmdDxGsaCompileShaderInput in;
  memset (&in, 0, sizeof (in));
  in.chipFamily = FAMILY_SI;
  in.chipRevision = SI_TAHITI_P_A0;

  AmdDxGsaCompileShaderOutput out;
  memset (&out, 0, sizeof (out));
  out.size = sizeof (out);

  HRESULT hr = compile (&in, &out);
  printf ("libgpuopen-amddxgsa - AmdDxGsaCompileShader(): 0x%08lX\n",
          (unsigned long) hr);

  if (out.pShaderBinary != NULL)
    free_shader (out.pShaderBinary);

  FreeLibrary (mod);
#else
  printf ("libgpuopen-amddxgsa - no system Direct3D 11 driver\n");
#endif
}
