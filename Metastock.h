#pragma once
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
struct MASTERHEADER {
	unsigned __int8 totalfx = 0;//max 0xff
	const char padding = 0;
	unsigned __int8 lastaddedfx = 0;//max 0xff 
	const char zero[50] = { 0 };
}tagMASTERHEADER;

struct MASTER {
	unsigned __int8 fx =  0 ;
	char unknown[2] = { 0 };
	unsigned __int8 fieldssize=24;//intradayda 7kolonsayisi*4=28
	unsigned __int8 fieldsnumber = 6;//intradayda 7kolon date-time-o-h-l-c-v
	char ___unknown[2] = { 0 };
	char secname[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
	char _unknown[2] = { 0 };
	char fdate[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char ldate[4] = { 0 };//YYMMDD ieee 
	char period[1] = { 'D' };//'I', 'D'
	char timeframe[2] = { 0 };//0 and 60 minutes
	char secsymbol[16] = { 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20 };
	char __unknown[1] = { 0 };
}tagMASTER;

struct XMASTERHEADER {
	unsigned char reserved[4] = { 0x5d,0xfe,0x58,0x4d };
	unsigned char unknown[6] = { 0 };
	unsigned __int16 totalfx_mwd =  0 ;//tersi
	unsigned char _unknown[2] = { 0 };
	unsigned __int16 _totalfx_mwd =  0 ;//tersi
	unsigned char __unknown[2] = { 0 };
	unsigned __int16 comingasfx_mwd = { 0 };//tersi//totalmwd+255+1
	unsigned char ___unknown[130] = { 0 };
}tagXMASTERHEADER;

#pragma pack(1)
struct XMASTER {
	char reserved1[1] = { 1 };
	char secsymbol[15] = { 0 };
	char secname[46] = { 0 };
	char period = 'D';//'I', 'D'
	unsigned __int16 timeframe = 0;//0 and 60 minutes
	unsigned __int16 fx =  0 ;//TERS�//256ilki
	unsigned __int32 _unknown = { 0x3f000000 };//BF intraday 3f daily
	char ___unknown[9] = { 0 };
	char notimportantfdate[4] = { 0 };//TERS� 20160229
	char ____unknown[20] = { 0 };
	unsigned __int32 fdate = { 0 };//TERS� 20160229
	unsigned __int32 _fdate = { 0 };//TERS� 20160229
	char _____unknown[4] = { 0 };
	unsigned __int32 ldate = { 0 };//TERS� 20160315
	unsigned char _______unknown[30] = { 0 };
	
}tagXMASTER;

struct FXHEADER {
	unsigned __int16 unkown =  0 ;
	unsigned __int16 totalrecord =  0 ;//ters+1 ieee de�il// +1 bo� sat�r
	char _unkown[20] = { 0 };
}tagFXHEADER;

struct FX {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
//	char time[4] = { 0 };//2359000 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFX;
struct FXIHEADER {
	unsigned __int16 unkown = 0;
	unsigned __int16 totalrecord = 0;//ters+1 ieee de�il// +1 bo� sat�r
	char _unkown[24] = { 0 };
}tagFXIHEADER;
struct FXI {
	char date[4] = { 0 };//YYMMDD ieee  YY=yyyy- 1900 
	char time[4] = { 0 };//235900 ieee
	char open[4] = { 0 };//ieee
	char high[4] = { 0 };//ieee
	char low[4] = { 0 };//ieee
	char close[4] = { 0 };//ieee
	char volume[4] = { 0 };//ieee

}tagFXI;
class Metastock
{
public:
	float BasicToIEEE(unsigned char *value)
	{
		float result;
		unsigned char *msbin = (unsigned char *)value;
		unsigned char *ieee = (unsigned char *)&result;
		unsigned char sign = 0x00;
		unsigned char ieee_exp = 0x00;
		int i;
		sign = msbin[2] & 0x80;
		for (i = 0; i<4; i++) ieee[i] = 0;
		if (msbin[3] == 0) return 0;
		ieee[3] |= sign;
		ieee_exp = msbin[3] - 2; 
		ieee[3] |= ieee_exp >> 1;
		ieee[2] |= ieee_exp << 7;
		ieee[2] |= msbin[2] & 0x7f;
		ieee[1] = msbin[1];
		ieee[0] = msbin[0];
		return (result);
	}

	bool IEEEToBasic(float *value, unsigned char *result)
	{
		unsigned char *ieee = (unsigned char *)value;
		unsigned char *msbin = (unsigned char *)result;
		unsigned char sign = 0x00;
		unsigned char msbin_exp = 0x00;
		int i;
		sign = ieee[3] & 0x80;
		msbin_exp |= ieee[3] << 1;
		msbin_exp |= ieee[2] >> 7;
		if (msbin_exp == 0xfe) return (FALSE);
		msbin_exp += 2; 
		for (i = 0; i<4; i++) msbin[i] = 0;
		msbin[3] = msbin_exp;
		msbin[2] |= sign;
		msbin[2] |= ieee[2] & 0x7f;
		msbin[1] = ieee[1];
		msbin[0] = ieee[0];
		return (TRUE);
	}
private:

	void FxYarat(const char* filename, std::vector<FX> fxs, std::vector<FXI> fxis,bool bintraday) {
		FXHEADER fxheader;
		FXIHEADER fxiheader;

		std::ofstream ofs;
		ofs.open(filename, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
		if (bintraday == TRUE) 
		{
			fxiheader.totalrecord = fxis.size() + 1;//+1 bir bo� sat�r
			ofs.write((const char*)&fxiheader, sizeof(fxiheader));
			ofs.write((const char*)fxis.data(), fxis.size() * sizeof(FXI));
		}
		else
		{
			fxheader.totalrecord = fxs.size() + 1;//+1 bir bo� sat�r
			ofs.write((const char*)&fxheader, sizeof(fxheader));
			ofs.write((const char*)fxs.data(), fxs.size() * sizeof(FX));
		}

		ofs.close();
	}

public:

	Metastock()
	{
	}

	virtual ~Metastock()
	{
	}


	bool WriteSecwithData(char* symbolname, std::vector<FX> fxs, std::vector<FXI> fxis , bool bintraday,int timeframe) {
		HANDLE hMFile = 0, hXMFile = 0;
		DWORD dwBytesWritten = 0;
		MASTERHEADER masterheader;
		MASTER master;
		XMASTERHEADER xmasterheader;
		XMASTER xmaster;
		


		
		if(_access("MASTER", 00) != 0)//Existence only- master yok
		{
			hMFile = CreateFile("MASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

			masterheader.totalfx = masterheader.lastaddedfx = 1;
			WriteFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);

			if (bintraday == TRUE) {
				master.fieldssize = 28;
				master.fieldsnumber = 7;
				master.period[0] = 'I';
				master.timeframe[0] = timeframe;
				memmove(master.fdate, fxis.front().date, 4);
				memmove(master.ldate, fxis.back().date, 4);
			}
			else
			{
				memmove(master.fdate, fxs.front().date, 4);
				memmove(master.ldate, fxs.back().date, 4);
			}
			master.fx = 1;
			memset(master.secname, 0x20, 16);
			memmove(master.secname, symbolname, lstrlen(symbolname));
			memset(master.secsymbol, 0x20, 16);
			memmove(master.secsymbol, symbolname, lstrlen(symbolname));
			WriteFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);

			FxYarat("F1.DAT", fxs, fxis, bintraday);
			CloseHandle(hMFile);
			return TRUE;
		}

		if (_access("MASTER", 06) != 0)//read-wirte- Read-only
		{
			hMFile = CreateFile("MASTER", GENERIC_READ , FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			
		}
		else
		{

		
			hMFile = CreateFile("MASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		

			ReadFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);
			ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			// Check for eof. sembolu ara
			while (dwBytesWritten != 0)
			{
				if (0 == strncmp(symbolname, master.secsymbol, lstrlen(symbolname)) && master.secsymbol[lstrlen(symbolname)]==0x20)
				{
						SetFilePointer(hMFile, -sizeof(MASTER), 0, FILE_CURRENT);
						if (bintraday == TRUE) {
							memmove(master.fdate, fxis.front().date, 4);
							memmove(master.ldate, fxis.back().date, 4);
						}
						else {
							memmove(master.fdate, fxs.front().date, 4);
							memmove(master.ldate, fxs.back().date, 4);
						}
						WriteFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);

					std::string a = "F" + std::to_string(master.fx) + ".DAT";
					FxYarat(a.c_str(), fxs, fxis, bintraday);
					CloseHandle(hMFile);
					return TRUE;
				}
				ReadFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);
			}

		
			//masterda sembol yok, yer musait yazilabilirde
		if ((masterheader.totalfx != 0xff) && (_access("MASTER", 06) == 0)) {
			


			SetFilePointer(hMFile, 0, 0, FILE_BEGIN);
			masterheader.totalfx = masterheader.lastaddedfx = masterheader.totalfx + 1;
			WriteFile(hMFile, &masterheader, sizeof(masterheader), &dwBytesWritten, NULL);

			SetFilePointer(hMFile, 0, 0, FILE_END);
			if (bintraday == TRUE) {
				memmove(master.fdate, fxis.front().date, 4);
				memmove(master.ldate, fxis.back().date, 4);
			}
			else
			{
				memmove(master.fdate, fxs.front().date, 4);
				memmove(master.ldate, fxs.back().date, 4);
			}
			master.fx = masterheader.totalfx;
			memset(master.secname, 0x20, 16);
			memmove(master.secname, symbolname, lstrlen(symbolname));
			memset(master.secsymbol, 0x20, 16);
			memmove(master.secsymbol, symbolname, lstrlen(symbolname));
			WriteFile(hMFile, &master, sizeof(master), &dwBytesWritten, NULL);

			std::string a = "F" + std::to_string(master.fx) + ".DAT";
			FxYarat(a.c_str(), fxs, fxis, bintraday);
			CloseHandle(hMFile);
			return TRUE;
		}
		else
		{
			CloseHandle(hMFile);
			if (_access("MASTER", 06) != 0)return FALSE;
			
		}

		

		if (_access("XMASTER", 00) != 0)//Existence only- xmaster yok
		{
			hXMFile = CreateFile("XMASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

			xmasterheader.comingasfx_mwd = 257;
			xmasterheader._totalfx_mwd = 1;
			xmasterheader.totalfx_mwd = 1;
			WriteFile(hXMFile, &xmasterheader, sizeof(xmasterheader), &dwBytesWritten, NULL);

			double f = 0.0, l = 0.0;
			if (bintraday == TRUE)
			{
				xmaster.period = 'I';
				xmaster.timeframe = timeframe;
				xmaster._unknown = 0xbf000000;
				f = BasicToIEEE((unsigned char*)fxis.front().date) + 19000000.0;
				l = BasicToIEEE((unsigned char*)fxis.back().date) + 19000000.0;
			}
			else
			{
				f = BasicToIEEE((unsigned char*)fxs.front().date) + 19000000.0;
				l = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0;
			}
			xmaster.fx = xmasterheader.comingasfx_mwd - 1;
			memmove(xmaster.secname, "", 46);
			memmove(xmaster.secname, symbolname, lstrlen(symbolname));
			memmove(xmaster.secsymbol, "", 15);
			memmove(xmaster.secsymbol, symbolname, lstrlen(symbolname));
			xmaster.fdate = f;
			xmaster._fdate = f;
			xmaster.notimportantfdate[0] = f;
			xmaster.ldate = l;
			WriteFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);

			FxYarat("F256.MWD", fxs, fxis, bintraday);
			CloseHandle(hXMFile);
			return TRUE;
		}
		else// xmaster  var a�
		{
			hXMFile = CreateFile("XMASTER", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		
		//xmaster ara
		ReadFile(hXMFile, &xmasterheader, sizeof(xmasterheader), &dwBytesWritten, NULL);
		ReadFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);
		// Check for eof.
		while (dwBytesWritten != 0)
		{
			if (0 == strncmp(symbolname, xmaster.secsymbol, lstrlen(symbolname)) && xmaster.secsymbol[lstrlen(symbolname)] == 0x0)
			{
				double f = 0.0, l = 0.0;
				SetFilePointer(hXMFile, -sizeof(xmaster), 0, FILE_CURRENT);
				if (bintraday==true)
				{
					f = BasicToIEEE((unsigned char*)fxis.front().date) + 19000000.0;
					l = BasicToIEEE((unsigned char*)fxis.back().date) + 19000000.0;
				}
				else
				{
					f = BasicToIEEE((unsigned char*)fxs.front().date) + 19000000.0;
					l = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0;
				}
				xmaster.fdate = f;
				xmaster._fdate = f;
				xmaster.notimportantfdate[0] = f;
				xmaster.ldate = l;
				WriteFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);

				std::string a = "F" + std::to_string(xmaster.fx) + ".MWD";
				FxYarat(a.c_str(), fxs, fxis, bintraday);
				CloseHandle(hXMFile);
				return TRUE;
			}
			ReadFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);
		}

		////yoksa xmastera ekle

		SetFilePointer(hXMFile, 0, 0, FILE_BEGIN);
		xmasterheader._totalfx_mwd = xmasterheader.comingasfx_mwd - 255;
		xmasterheader.totalfx_mwd = xmasterheader.comingasfx_mwd - 255;
		xmasterheader.comingasfx_mwd = xmasterheader.comingasfx_mwd + 1;
		WriteFile(hXMFile, &xmasterheader, sizeof(xmasterheader), &dwBytesWritten, NULL);

		SetFilePointer(hXMFile, 0, 0, FILE_END);
		double f = 0.0, l = 0.0;
		if (bintraday == TRUE)
		{
			xmaster.period = 'I';
			xmaster.timeframe = timeframe;
			xmaster._unknown = 0xbf000000;
			f = BasicToIEEE((unsigned char*)fxis.front().date) + 19000000.0;
			l = BasicToIEEE((unsigned char*)fxis.back().date) + 19000000.0;
		}
		else
		{
			f = BasicToIEEE((unsigned char*)fxs.front().date) + 19000000.0;
			l = BasicToIEEE((unsigned char*)fxs.back().date) + 19000000.0;
		}
		xmaster.fx = xmasterheader.comingasfx_mwd - 1;
		memmove(xmaster.secname, "",46);
		memmove(xmaster.secname, symbolname, lstrlen(symbolname));
		memmove(xmaster.secsymbol, "", 15);
		memmove(xmaster.secsymbol, symbolname, lstrlen(symbolname));
		xmaster.fdate = f;
		xmaster._fdate = f;
		xmaster.notimportantfdate[0] = f;
		xmaster.ldate = l;
		WriteFile(hXMFile, &xmaster, sizeof(xmaster), &dwBytesWritten, NULL);

		std::string a = "F" + std::to_string(xmaster.fx) + ".MWD";
		FxYarat(a.c_str(), fxs, fxis, bintraday);
		CloseHandle(hXMFile);
		return TRUE;

	}

};

