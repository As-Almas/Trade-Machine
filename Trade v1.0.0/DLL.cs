using System.Runtime.InteropServices;
using System;
//using System;



namespace Trade_v1_0_0
{
    class DLL
    {
        private IntPtr LoadedDLL = IntPtr.Zero;
        

        [DllImport("kernel32.dll")]
        static extern IntPtr LoadLibrary(string path);

        [DllImport("kernel32.dll")]
        static extern void FreeLibrary(IntPtr dll);

        [DllImport("kernel32.dll")]
        static extern IntPtr GetProcAddress(IntPtr dll, string procName);

        public DLL(string path)
        {
            LoadedDLL = LoadLibrary(path);
            if(IntPtr.Zero == this.LoadedDLL)
            {
                throw new Exception("Dll '" + path + "' is not found!");
            }
        }

        public Delegate GetFunctionAddress(string funcName, Type type)
        {
            return Marshal.GetDelegateForFunctionPointer(GetProcAddress(this.LoadedDLL, funcName), type);
        } 

        ~DLL()
        {
            FreeLibrary(this.LoadedDLL);
        }
    }
}
