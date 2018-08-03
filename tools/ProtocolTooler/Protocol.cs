using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProtocolTooler
{
    /**
     * 
     * 协议结构：start_flag + len + frame_id + FCtrl + dst_network_addr + src_network_addr + payload + crc
     *
     * 
     * */
    class Protocol
    {  
        //标志位
        private short start_flag = 0;

        //frameId 到crc的长度
        private short len = 0;

        //frameId
        private byte frame_id = 0;

        //帧头控制字
        private byte f_ctrl = 0;

        //目的网络地址
        private short dst_network_addr = 0;

        //来源网络地址
        private short src_network_addr = 0;

        //以下为payload数据部分

        private byte p_ctrl = 0;

        private bool bHaveP2p = false;

        private byte total_frame_size = 0;

        private byte current_frame = 0;

        private byte p2p_data_id = 0;

        private short p2p_data_len = 0;
        private byte[] p2p_data = new byte[15];

        //数据内容 p_data_len 包含data_id长度
        private short p_data_len = 0;
        private byte p_data_id = 0;
        private byte[] p_data = new byte[65535];

        public byte getTotalFrameSize()
        {
            return total_frame_size;
        }

        public void setTotalFrameSize(byte b)
        {
            this.total_frame_size = b;
        }

        public byte getCurrentFrame()
        {
            return current_frame;
        }

        public void setCurrentFrame(byte b)
        {
            this.current_frame = b;
        }

        public void setbDivPack(bool security)
        {
            //标识bit位置
            int index = 4;

            if (security)
            {
                p_ctrl = (byte)(p_ctrl | (0x01 << index));
            }
            else
            {
                p_ctrl = (byte)(p_ctrl & (~(0x01 << index)));
            }
        }

        public bool getbDivPack()
        {
            int index = 4;

            return (p_ctrl >> index & 0x01) == 1 ? true : false;
        }

        public void setbHaveP2p(bool b)
        {
            bHaveP2p = b;
        }

        public bool getbHaveP2p()
        {
            return bHaveP2p;
        }

        public short getLen()
        {
            return this.len;
        }

        public void setStartFlag(short s)
        {
            this.start_flag = s;
        }

        public short getStartFlag()
        {
            return this.start_flag;
        }

        public void setFrameId(byte frameId)
        {
            this.frame_id = frameId;
        }

        public byte getFrameid()
        {
            return this.frame_id;
        }

        public void setAckFlag(bool ackFlag)
        {
            //标识bit位置
            int index = 1;

            if (ackFlag)
            {
                f_ctrl = (byte)(f_ctrl | (0x01 << index));
            }
            else
            {
                f_ctrl = (byte)(f_ctrl & (~(0x01 << index)));
            }
        }

        /// <summary>
        /// 获取应答帧标识
        /// </summary>
        /// <returns></returns>
        public bool getAckFlag()
        {
            int index = 1;

            return (f_ctrl >> index & 0x01) == 1 ? true : false;
        }

        public void setProtocolType(byte b)
        {
            //先归零
            f_ctrl = (byte)(f_ctrl & 0x1F);

            //赋值
            f_ctrl = (byte)( (b<<5) | f_ctrl );
        }

        public byte getProtocolType()
        {
            return (byte)( (f_ctrl & 0xE0) >> 5  );
        }

        /// <summary>
        /// 设置应答请求
        /// </summary>
        /// <param name="ackReq"></param>
        public void setAckReq(bool ackReq)
        {
            //标识bit位置
            int index = 0;

            if(ackReq)
            {
                f_ctrl = (byte)(f_ctrl | (0x01 << index));
            }
            else
            {
                f_ctrl = (byte)(f_ctrl & (~(0x01 << index)));
            }
        }

        /// <summary>
        /// 获取应答请求
        /// </summary>
        /// <returns></returns>
        public bool getAckReq()
        {
            int index = 0;

            return ( f_ctrl >> index & 0x01 ) == 1 ? true : false;
        }

        /// <summary>
        /// 获取帧头控制字值 
        /// </summary>
        /// <returns></returns>
        public byte getFCtrl()
        {
            return this.f_ctrl;
        }

        public byte getPCtrl()
        {
            return this.p_ctrl;
        }

        public void setP2pDataId(byte b)
        {
            this.p2p_data_id = b;
        }

        public byte getP2pDataId()
        {
            return this.p2p_data_id;
        }

        public void setDataId(byte b)
        {
            this.p_data_id = b;
        }

        public byte getDataId()
        {
            return this.p_data_id;
        }

        public void setData(byte[] b)
        {
            p_data_len = 0;

            if (null == b || b.Length == 0)
            {
                return;
            }

            for (int i = 0; i < b.Length; i++)
            {
                p_data[i] = b[i];
            }

            p_data_len = (short)(b.Length);
        }

        public byte[] getData()
        {
            if (p_data_len == 0)
            {
                return null;
            }

            byte[] b = new byte[p_data_len];

            for (int i = 0; i < p_data_len; i++)
            {
                b[i] = p_data[i];
            }

            return b;
        }

        public void setP2pData(byte[] b)
        {
            p2p_data_len = 0;

            if (null == b || b.Length == 0)
            {
                return;
            }

            for(int i = 0;i<b.Length && i<15;i++)
            {
                p2p_data[i] = b[i];
            }

            p2p_data_len = (short)(b.Length);
        }

        public byte[] getP2pData()
        {
            if(p_data_len == 0)
            {
                return null;
            }

            byte[] b = new byte[p2p_data_len];

            for(int i = 0; i < p2p_data_len; i++)
            {
                b[i] = p2p_data[i];
            }

            return b;
        }

        /// <summary>
        /// 设置来源地址
        /// </summary>
        /// <param name="src_network_addr"></param>
        public void setSrcNetworkAddr(short src_network_addr)
        {
            this.src_network_addr = src_network_addr;
        }

        /// <summary>
        /// 获取来源地址
        /// </summary>
        /// <returns></returns>
        public short getSrcNetworkAddr()
        {
            return this.src_network_addr;
        }

        /// <summary>
        /// 设置目标地址
        /// </summary>
        /// <param name="dst_network_addr"></param>
        public void setDstNetworkAddr(short dst_network_addr)
        {
            this.dst_network_addr = dst_network_addr;
        }

        /// <summary>
        /// 获取目标地址
        /// </summary>
        /// <returns></returns>
        public short getDstNetworkAddr()
        {
            return this.dst_network_addr;
        }

        public void setSecurity(bool security)
        {
            //标识bit位置
            int index = 7;

            if (security)
            {
                p_ctrl = (byte)(p_ctrl | (0x01 << index));
            }
            else
            {
                p_ctrl = (byte)(p_ctrl & (~(0x01 << index)));
            }
        }

        public bool getSecurity()
        {
            int index = 7;

            return (p_ctrl >> index & 0x01) == 1 ? true : false;
        }

        public bool isDivPack()
        {
            int index = 4;

            return (p_ctrl >> index & 0x01) == 1 ? true : false;
        }

        public byte getP2pLen()
        {
            return (byte)(p_ctrl & 0x0f);
        }

        /// <summary>
        /// 设置帧数据
        /// </summary>
        /// <param name="data"></param>
        /// <param name="msg"></param>
        /// <returns></returns>
        public int setBytes(byte[] data,out String msg)
        {
            msg = "";

            if (data == null || data.Length < 10)
            {
                msg = "data为空或者data长度不合法 data.Length ："+ data.Length;
                return 1;
            }
            //判断标志位
            if(data[0] != 0x7e || data[1] != 0x7e )
            {
                msg = "数据不是从0x7e0x7e开始的 分别为:"+data[0].ToString("X2")+ " "+ data[1].ToString("X2");
                return 1;
            }

            //获取长度
            len = bytesToShort(data[2],data[3]);

            if((len + 2 + 2 + 2) != data.Length)
            {
                msg = "数据中长度和数据内容长度不匹配，数据中长度："+len+" 数据内容长度:"+(data.Length - 2 -2 -2);
                return 1;
            }

            //校验CRC
            short crc = bytesToShort(data[data.Length - 2], data[data.Length - 1]);

            short checkCrc = getCRC16(data,4,len);

            if(crc != checkCrc)
            {
                msg = "crc校验不匹配,内容中crc："+crc+" 计算所得crc："+checkCrc;
                return 1;
            }

            //获取frameid
            frame_id = data[4];

            //获取帧头控制字
            f_ctrl = data[5];

            //获取dst
            dst_network_addr = bytesToShort(data[6],data[7]);

            //获取src
            src_network_addr = bytesToShort(data[8],data[9]);

            //获取p_ctrl
            p_ctrl = data[10];

            short data_pos = 11;

            //先判断是否有p2p
            byte p2pLen = (byte)(p_ctrl & 0x0F);

            if (data.Length - data_pos - 2 < p2pLen)
            {
                msg = "数据长度异常1，无法正确解析";
                return 1;
            }

            if (p2pLen > 0 )
            {
                bHaveP2p = true;
                p2p_data_len = (byte)(p2pLen - 1);

                p2p_data_id = data[data_pos++];

                if (data.Length - data_pos - 2 -1 < p2p_data_len)
                {
                    msg = "数据长度异常2，无法正确解析";
                    return 1;
                }

                for (int i = 0;i<p2p_data_len;i++)
                {
                    p2p_data[i] = data[data_pos++];
                }
            }
            
            if(isDivPack())
            {
                if (data.Length - data_pos - 2 - 1 < 2)
                {
                    msg = "数据长度异常3，无法正确解析";
                    return 1;
                }

                total_frame_size = data[data_pos++];
                current_frame = data[data_pos++];
            }

            p_data_len = (short)(len - 1 - 1 - 2 - 2 - 1);

            if(p_data_len <= 0)
            {
                msg = "没发现data数据";
                return 1;
            }

            if (data_pos >= data.Length)
            {
                msg = "数据内容的位置大于数据长度 data_pos:"+data_pos+" 长度："+data.Length;
                return 1;
            }

            p_data_len -= p2pLen;

            if(isDivPack())
            {
                p_data_len -= 2;
            }

            p_data_id = data[data_pos++];
            //因为包含了data_id位置
            p_data_len--;

            for(int i = 0;i< p_data_len; i++)
            {
                p_data[i] = data[data_pos++];
            }

            return 0;
        }

        public byte[] getBytes()
        {
            byte[] retBytes = new byte[1024];

            int index = 0;

            //设置开始标记
            byte [] startBytes = shortToBytes(start_flag);
            
            //必须确保位置不变
            if(startBytes != null && startBytes.Length >= 2)
            {
                retBytes[index++] = startBytes[0];
                retBytes[index++] = startBytes[1];
            }
            else
            {
                retBytes[index++] = 0;
                retBytes[index++] = 0;
            }

            len = (short)(1 + 1 + 2 + 2 + 1);
            
            if(getbDivPack())
            {
                len += 2;
            }

            if(bHaveP2p)
            {
                len += (short)(1+p2p_data_len);
            }

            len += (short)(1+p_data_len);

            //设置长度
            byte[] lenBytes = shortToBytes(len);

            if (lenBytes != null && lenBytes.Length >= 2)
            {
                retBytes[index++] = lenBytes[0];
                retBytes[index++] = lenBytes[1];
            }
            else
            {
                retBytes[index++] = 0;
                retBytes[index++] = 0;
            }

            //设置帧ID
            retBytes[index++] = frame_id;

            //设置帧控制字
            retBytes[index++] = f_ctrl;

            //设置目的地址
            byte[] dstBytes = shortToBytes(dst_network_addr);

            if (dstBytes != null && dstBytes.Length >= 2)
            {
                retBytes[index++] = dstBytes[0];
                retBytes[index++] = dstBytes[1];
            }
            else
            {
                retBytes[index++] = 0;
                retBytes[index++] = 0;
            }

            //设置来源地址
            byte[] srcBytes = shortToBytes(src_network_addr);

            if (srcBytes != null && srcBytes.Length >= 2)
            {
                retBytes[index++] = srcBytes[0];
                retBytes[index++] = srcBytes[1];
            }
            else
            {
                retBytes[index++] = 0;
                retBytes[index++] = 0;
            }



            //计算数据头部

            p_ctrl = (byte)(p_ctrl & 0xF0);

            if(bHaveP2p)
            {
                p_ctrl = (byte)( p_ctrl | (1+p2p_data_len) );
            }

            //设置数据头
            retBytes[index++] = p_ctrl;

            if(bHaveP2p)
            {
                //放p2p dataid
                retBytes[index++] = p2p_data_id;
                for(int i = 0;i<p2p_data_len;i++)
                {
                    retBytes[index++] = p2p_data[i];
                }
            }

            if(getbDivPack())
            {
                retBytes[index++] = total_frame_size;
                retBytes[index++] = current_frame;
            }

            //设置数据ID
            retBytes[index++] = p_data_id;

            if (p_data_len > 0)
            {
                //设置数据部分
                for (int i = 0; i < p_data_len; i++)
                {
                    retBytes[index++] = p_data[i];
                }
            }           

            //总长度
            short crc = getCRC16(retBytes,2+2, index-2-2);

            //设置crc
            byte[] crcBytes = shortToBytes(crc);

            if (crcBytes != null && crcBytes.Length >= 2)
            {
                retBytes[index++] = crcBytes[0];
                retBytes[index++] = crcBytes[1];
            }
            else
            {
                retBytes[index++] = 0;
                retBytes[index++] = 0;
            }

            byte[] endBytes = new byte[index];

            for(int i = 0;i<endBytes.Length;i++)
            {
                endBytes[i] = retBytes[i];
            }

            return endBytes;
        }

        public byte[] shortToBytes(short s)
        {
            byte[] b = new byte[2];
            b[0] = (byte)s;
            b[1] = (byte)(s >> 8);

            return b;
        }

        public short bytesToShort(byte L,byte H)
        {
            return (short)( (0x0000 | H)<<8  | L);
        }

        override
        public String ToString()
        {
            //start_flag + len + frame_id + FCtrl + dst_network_addr + src_network_addr + payload + crc
            return "start_flag:" + shortToHexString(start_flag) + " len:" + shortToHexString(len) + " frame_id:" + byteToHexString(frame_id) + " f_ctrl:" + byteToHexString(f_ctrl)+ " dst:" + shortToHexString(dst_network_addr)
                + " src:" + shortToHexString(src_network_addr) + " p_ctrl:"+byteToHexString(p_ctrl)+" data_id:"+byteToHexString(p_data_id)+" data:"+bytesToHexString(p_data,0,p_data_len);
        }

        public static String bytesToHexString(byte[] b,int begin_pos,int len)
        {
            if(null == b)
            {
                return "";
            }

            String s = "";

            for(int i = 0;i<len;i++)
            {
                s += byteToHexString(b[begin_pos+i]);
            }

            return s;
        }

        public static String byteToHexString(byte b)
        {
            return b.ToString("X2");
        }

        public static String shortToHexString(short s)
        {
            byte L = (byte)s;

            byte H = (byte)(s >> 8);

            return L.ToString("X2") + H.ToString("X2");
        }

        private byte[] g_CrcHByte =
        {
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40
        };

        private byte[] g_CrcLByte =
        {
            0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
            0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
            0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
            0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
            0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
            0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
            0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
            0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
            0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
            0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
            0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
            0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
            0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
            0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
            0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
            0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
            0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
            0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
            0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
            0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
            0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
            0x41, 0x81, 0x80, 0x40
        };

        public short getCRC16(byte[] pData, int begin_pos,int lenth)
        {
            byte crcH = 0xFF;
            byte crcL = 0xFF;

            int index = 0;

            while ((lenth--) != 0)
            {
                byte i = (byte)(crcL ^ pData[begin_pos + (index++)]);
                crcL = (byte)(crcH ^ g_CrcHByte[i]);
                crcH = g_CrcLByte[i];
            }
            return (short)(crcH << 8 | crcL);
        }
    }
}
