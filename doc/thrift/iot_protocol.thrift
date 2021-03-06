namespace java com.hc.iot

enum eIOTSignalType {
	E_Signal_NoThing         = 0,
	E_Signal_InitDB          = 1
}

enum eIOTNodeType {
	E_Type_Default           = 0,
	E_Type_M_AP              = 10,
	E_Type_R_AP              = 11,
	E_Type_C_AP              = 12,
	E_Type_B_AP              = 13,
	E_Type_L_AP              = 14,
	E_Type_T_AP              = 15,
	E_Type_Muti_AP           = 16,
	E_Type_N_ED              = 30,
	E_Type_LP_ED             = 31,
	E_Type_M_ED              = 32,
	E_Type_MLP_ED            = 33
}

service LogicFunctionService{

	i32 raiseSignal(1:eIOTSignalType type)
	
	map<string,eIOTNodeType> getAllowSites()
	
	i32 addSite(string phyAddr,eIOTNodeType type)
	
	i32 delSite(string phyAddr)
	
	i32 delDevice(string phyAddr)
	
	i32 getNetworkStatus();
	
	i32 operateNetwork(bool isOpen)
	
	i32 rebuildNetwork()
	
	i32 sendCommand()
}
