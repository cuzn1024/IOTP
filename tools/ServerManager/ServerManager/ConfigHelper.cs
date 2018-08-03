using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServerManager
{
    public class ConfigHelper
    {
        #region API函数声明

        [DllImport("kernel32")]//返回0表示失败，非0为成功
        private static extern long WritePrivateProfileString(string section, string key,
            string val, string filePath);

        [DllImport("kernel32")]//返回取得字符串缓冲区的长度
        private static extern int GetPrivateProfileString(string section, string key, string def, byte[] retVal, int size, string filePath);
        //         private static extern long GetPrivateProfileString(string section, string key,
        //             string def, StringBuilder retVal, int size, string filePath);


        #endregion

        #region 读Ini文件

        public static string ReadIniData(string Section, string Key, string NoText, string iniFilePath)
        {
            if (File.Exists(iniFilePath))
            {
                //StringBuilder temp = new StringBuilder(1024);
                Byte[] Buffer = new Byte[65535];
                //GetPrivateProfileString(Section, Key, NoText, temp, 1024, iniFilePath);
                //return temp.ToString();
                int bufLen = GetPrivateProfileString(Section,Key,NoText, Buffer, Buffer.GetUpperBound(0),iniFilePath);
                //必须设定0（系统默认的代码页）的编码方式，否则无法支持中文  
                string s = Encoding.GetEncoding(0).GetString(Buffer);
                s = s.Substring(0, bufLen);
                return s.Trim();
            }
            else
            {
                return String.Empty;
            }
        }

        #endregion

        #region 写Ini文件

        public static bool WriteIniData(string Section, string Key, string Value, string iniFilePath)
        {
            if (File.Exists(iniFilePath))
            {
                long OpStation = WritePrivateProfileString(Section, Key, Value, iniFilePath);
                if (OpStation == 0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        #endregion

        //读取指定的Section的所有Value到列表中  
        public static void ReadSectionValues(string Section, NameValueCollection Values,string filePath)
        {
            StringCollection KeyList = new StringCollection();
            ReadSection(Section, KeyList, filePath);
            Values.Clear();
            foreach (string key in KeyList)
            {
                Values.Add(key, ReadIniData(Section, key, "",filePath));

            }
        }

        //从Ini文件中，将指定的Section名称中的所有Ident添加到列表中  
        private static void ReadSection(string Section, StringCollection Idents,string filePath)
        {
            Byte[] Buffer = new Byte[16384];
            //Idents.Clear();  

            int bufLen = GetPrivateProfileString(Section, null, null, Buffer, Buffer.GetUpperBound(0), filePath);
            //对Section进行解析  
            GetStringsFromBuffer(Buffer, bufLen, Idents);
        }

        private static void GetStringsFromBuffer(Byte[] Buffer, int bufLen, StringCollection Strings)
        {
            Strings.Clear();
            if (bufLen != 0)
            {
                int start = 0;
                for (int i = 0; i < bufLen; i++)
                {
                    if ((Buffer[i] == 0) && ((i - start) > 0))
                    {
                        String s = Encoding.GetEncoding(0).GetString(Buffer, start, i - start);
                        Strings.Add(s);
                        start = i + 1;
                    }
                }
            }
        }
    }
}
