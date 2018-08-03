using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace ServerManager
{
    class Tools
    {
        public static void log(String s)
        {
            Console.WriteLine(s);
        }

        public static int getTimeStamp()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return (int)(Convert.ToInt64(ts.TotalMilliseconds) / 1000);
        }

        public static byte[] HexStringToBytes(string hexString)
        {
            if (hexString.Length == 0)
            {
                return null;
            }

            hexString = hexString.Replace(" ", "");

            if ((hexString.Length % 2) != 0)
            {
                return null;
            }

            byte[] returnBytes = new byte[hexString.Length / 2];
            for (int i = 0; i < returnBytes.Length; i++)
            {
                returnBytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            }
            return returnBytes;
        }

        public static bool isHexString(String s)
        {
            return Regex.IsMatch(s, "^[0-9A-Fa-f]+$");
        }
    }
}
