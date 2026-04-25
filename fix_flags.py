# Global names provided by PlatformIO (defined here to suppress linting errors)
if "Import" not in globals():
    def Import(*args, **kwargs): pass
if "env" not in globals():
    env = None

Import("env")

# Flags que são válidas apenas para C++
cpp_only_flags = [
    "-fno-exceptions",
    "-fno-rtti",
    "-Wno-deprecated-enum-enum-conversion"
]

# Adiciona apenas ao ambiente de C++ (CXXFLAGS)
env.Append(CXXFLAGS=cpp_only_flags)

# Patch para corrigir avisos no OneWire.cpp do ESP32
import os
one_wire_path = os.path.join(env.subst("$PROJECT_LIBDEPS_DIR"), env.subst("$PIOENV"), "OneWire", "OneWire.cpp")
if os.path.exists(one_wire_path):
    with open(one_wire_path, "r", encoding="utf-8") as f:
        content = f.read()
    
    if "#  undef noInterrupts() {" in content:
        content = content.replace(
            "#  undef noInterrupts() {portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;portENTER_CRITICAL(&mux)",
            "#  undef noInterrupts"
        )
        content = content.replace(
            "#  undef interrupts() portEXIT_CRITICAL(&mux);}",
            "#  undef interrupts"
        )
        with open(one_wire_path, "w", encoding="utf-8") as f:
            f.write(content)
        print("\n[PATCH] OneWire.cpp corrigido para remover avisos de #undef.\n")

print("\n[FIX_FLAGS] Flags especificas de C++ aplicadas com sucesso ao CXXFLAGS.\n")
