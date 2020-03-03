// ArtMoney.cpp : main project file.

#include "stdafx.h"
#include "AWE.h"
#include "Serial.h"
#include "Form2.h"
#include "Form1.h"
#include "IM1.h"
#include "PVR1.h"
#include "IVL1.h"
#include "KOM1.h"
#include "windows.h"
#include "math.h"
#include "sharedmemory.h"


//double z = 0;

using namespace ArtMoney;

/*void asdasd()
{
    next:
	Sleep(100);
	z += 0.1;
	*buf_analog_out = (float)sin(z);
	*buf_discrete_out = (int)z;
	if (z > 5000) z = 0;
	goto next;
}
*/
[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	InitSharedMemory();
	int* mas_int_in = new int[100];
	float* mas_float_in = new float[100];

	int* mas_int_out = new int[100];
	float* mas_float_out = new float[100];

	for (int i = 0; i < 100; i++)
	{
		mas_int_in[i] = 0;
		mas_float_in[i] = 0;
		mas_int_out[i] = 0;
		mas_float_out[i] = 0;
	}

	
	next:
	mas_int_out[0]++;
	mas_float_out[0]=sin(mas_int_out[0]);
	
	write_alldata_to_raek(mas_int_out, mas_float_out);
	read_alldata_from_raek(mas_int_in, mas_float_in);

	Sleep(200);
	goto next;





	//CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)asdasd,NULL,NULL,NULL);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());

	return 0;
}

System::Void Form1::button1_Click(System::Object^  sender, System::EventArgs^  e) 
 {
	 this->Close();
 }
System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	try
	{
		System::Globalization::CultureInfo^ ci = System::Globalization::CultureInfo::CurrentCulture;
		this->separator = ci->NumberFormat->NumberDecimalSeparator;
		nobi = gcnew List<Form^>(0);
		splitlist = gcnew List<String^>();

		// Указатель на функцию GetAdaptersInfo
		typedef DWORD(CALLBACK* PTR_GETADAPTERSINFO)(PIP_ADAPTER_INFO, PULONG);

		//FILE* out1 = fopen("c:\\ipmac.txt", "w");

		// Обращение iphlpapi.dll
		HINSTANCE iphlpapi;
		iphlpapi = LoadLibraryA("iphlpapi.dll");

		if (!iphlpapi)
		{
			this->Close();
			//fprintf(out1,"iphlpapi.dll not found\n");
		}

		PTR_GETADAPTERSINFO GetAdaptersInfo;
		GetAdaptersInfo = (PTR_GETADAPTERSINFO)GetProcAddress(iphlpapi, "GetAdaptersInfo");
		ULONG adapter_info_size = 0;
		PIP_ADAPTER_INFO ptr_adapter_info = NULL;
		PIP_ADAPTER_INFO ptr_adapter_info_first = NULL;

		GetAdaptersInfo(ptr_adapter_info, &adapter_info_size);

		ptr_adapter_info_first = ptr_adapter_info = (PIP_ADAPTER_INFO) new(char[adapter_info_size]);

		if (GetAdaptersInfo(ptr_adapter_info, &adapter_info_size) != ERROR_SUCCESS)
		{

			//fprintf(out1, "Error while GetAdaptersInfo\n" );
			delete(ptr_adapter_info);
			this->Close();
		}

		bool alien = true;
		char mac[70][18] = { "78 24 af db df 80", // Саньков (рабочий)
							"74 d0 2b e1 97 cf", // Казаченок (рабочий)
							"b8 ac 6f 56 42 d4", // Саньков (домашний)
							"b8 88 e3 b9 32 65", // Бурнашов (домашний)
							"c8 d9 d2 91 24 75", // Коватев (рабочий)
							"54 53 ed ad 3b 65", // Парфенчиков (рабочий)
							"c8 d9 d2 91 21 11", // Михайлов (рабочий)
							"54 53 ed ae e6 17", // Волков (рабочий)
							"54 53 ed bb aa ac", // Коватева (рабочий)
							"54 53 ed ae ec eb", // Коватева (рабочий)
							"54 a0 50 38 52 9c", // Коватева (рабочий)
							"54 53 ed ae dc 48", // Мезенцев (рабочий)
							"c8 d9 d2 91 26 f4", // Шенцов (рабочий)
							"98 54 1b a2 ed a6", // Бурнашов (рабочий)
							"54 53 ed 3c 01 72", // Карпенко (рабочий)
							"78 84 3c cb 85 40", // Белянин (рабочий)
							"04 7d 7b af 9e aa", // Белянина (домашний)
							"40 b0 34 c0 39 a6", // Карпенко Ю. (рабочий)
							"88 ae 1d 20 50 5a", // Елисеев К. (домашний)
							"18 cf 5e c1 78 9f", // Балтинский (рабочий)
							"18 cf 5e 99 e7 53", // Чакин (рабочий)
							"d8 9c 67 05 58 af", // Подгорнов (рабочий)
							"6c 4b 90 34 8b bd", // Подгорнов (рабочий стац)
							"c8 d9 d2 91 26 f6", // Шевяков (рабочий)
							"6c 4b 90 36 b3 b1", // Шевяков (стационарный)
							"00 1f 3b 1e 78 d5", // Маслов (рабочий)
							"40 b0 34 c0 3c b0", // Нерозников (рабочий)
							"6c 4b 90 36 b2 b4", // Водопьянов (рабочий стац)
							"ac 22 0b 56 55 d7", // Водопьянов (рабочий ноут)
							"48 5a b6 c5 09 c5", // Смена ЦТАИ (рабочий 1)
							"80 56 f2 63 93 79", // Смена ЦТАИ (рабочий 2)
							"7c d3 0a 30 95 f4", // Смена ЦТАИ (рабочий 3)
							"24 0a 64 79 55 83", // Кувшинов (рабочий)
							"24 0a 64 79 55 15", // Борисов (рабочий)
							"3c d9 2b 57 ab 0c", // Смена ЦТАИ (рабочий 3)
							"3c 97 0e a2 8e 68", // А. З. (рабочий ноут)
							"00 03 2d 24 58 0a", // А. З. (стационарный)
							"40 b0 34 c0 3c 9b", // Васильев (рабочий)
							"40 b0 34 c0 3e 4a", // Кочнев (рабочий)
							"18 cf 5e 99 aa e6", // Лысенко Роман (рабочий)
							"18 cf 5e 7e 84 e3", // Добыш Стас (рабочий)
							"40 b0 34 c0 3d 58", // Добыш Наташа (рабочий)
							"4c 0f 6e da 7a 71", // Козлов (рабочий)
							"18 cf 5e c0 ef 7a", // Абдрахманов (рабочий)
							"40 b0 34 c0 3b 86", // Иванов (рабочий)
							"74 d0 2b e1 98 80", // Белан (рабочий)
							"98 54 1b c9 28 59", // Оладышев Павел (рабочий)
							"24 0a 64 17 b7 ff", // Ширяев Илья (рабочий)
							"54 a0 50 38 52 95", // Долгополов (рабочий)
							"6c 4b 90 34 90 51", // Долгополов (рабочий стац)
							"18 cf 5e c0 ef 79", // Селезнев (рабочий)
							"2c 27 d7 47 3a 2d", // ЦТАИ Янковский (рабочий)
							"00 21 85 14 84 75", // Дьяков (рабочий)
							"80 19 34 13 67 5d", // Цыбульский (рабочий)
							"74 d0 2b 6b 61 bf", // Кувшинов Н. (рабочий)
							"98 54 1b c9 2a 60", // Кривцов В. (рабочий)
							"6c 4b 90 36 b9 99", // Рыбинцев Е. (рабочий стац)
							"6c 4b 90 36 37 03", // Крюков В. (рабочий стац)
							"6c 4b 90 34 8b 5e", // Шмырев Д. (рабочий стац)
							"54 53 ed ad 3a 3c", // Губин Р. (рабочий)
							"6c 4b 90 34 8b f6", // Пауль А. (рабочий стац)						
							"54 53 ed ae ea 6e", // Уварова А. (рабочий)
							"c8 d9 d2 8c 12 6a", // Уварова А. (рабочий стац)
							"6c 4b 90 34 8b 3d", // Губарь А. (рабочий стац)
							"c8 d9 d2 89 58 f3", // Губарь А. (рабочий ноут)
							"bc 5f f4 40 b4 df", // Парфенчиков (рабочий стац)
							"6c 4b 90 36 b8 27", // Тарасов (рабочий стац)
							"c8 d9 d2 91 29 f4", // Приписнов (домашний)
							"18 cf 5e c1 45 7d", // Приписнов (стац)
							"c4 85 08 d3 34 17" }; // Приписнов (рабочий)

	

		while (ptr_adapter_info)
		{
			//fprintf (out1, "ID of adapter: %s\n", ptr_adapter_info->AdapterName );
			//fprintf (out1, "Description: %s\n", ptr_adapter_info->Description );
			//fprintf (out1, "MAC address: " );
			char mact[18];

			for (char i = 0; i < (int)ptr_adapter_info->AddressLength; i++)
			{
				sprintf(&mact[i * 3], "%02x ", (unsigned char)ptr_adapter_info->Address[i]);
				//fprintf(out1,"%02x ", (unsigned char)ptr_adapter_info->Address[i]);
				//if ()
			}
			mact[17] = '\0';

			for (int i = 0; i < 70; i++) if (strcmp(mact, mac[i]) == 0) alien = false;

			//fprintf (out1, "\n");
			//fprintf (out1,"Adapter type: %u\n\n", ptr_adapter_info->Type );

			ptr_adapter_info = ptr_adapter_info->Next;

		}
		delete(ptr_adapter_info_first);
		if (alien) std::memset(rus, 0, 256); //this->Close();


		std::memset(rus,0,256);
		for (int i=0; i<66; i++) rus[ (unsigned char)getalf[i] ] = (unsigned char)rusalf[i];

		canvBitmap = gcnew Bitmap(this->pictureBox1->Width, this->pictureBox1->Height); 
		pen1 = gcnew Pen(Color::Black, 1);
		pen2 = gcnew Pen(Color::Black, 1);
		fg1 = this->pictureBox1->CreateGraphics();
		g1 = Graphics::FromImage(canvBitmap); 
		myforecolor = gcnew Color();
		myforecolor = Color::WhiteSmoke;

		if (tabControl1->SelectedIndex == 0)
		{
			if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 420.0f / 297.0f) mas = this->pictureBox1->Width / 420.0f;
			else mas = this->pictureBox1->Height / 297.0f;
			
			font1 = gcnew System::Drawing::Font("MS Sans Serif", 2.0f*mas);
		}
		else if (tabControl1->SelectedIndex == 1)
		{
			if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 1112.0f / 782.0f) mas = this->pictureBox1->Width / 1112.0f;
			else mas = this->pictureBox1->Height / 782.0f;
			dx = -556.0f*mas; dy = 391.0f*mas;
			font1 = gcnew System::Drawing::Font("MS Sans Serif", 6.0f*mas);
		}
		
		this->label1->Text = Convert::ToString(dx);
		this->label2->Text = Convert::ToString(dy);
		this->label3->Text = Convert::ToString(mas);

		hMod=GetModuleHandle(NULL);
		if(!hMod) {}							// здесь могла быть обработка ошибок ))

		FILE* fav1;
		fav1=fopen("fav.txt","r");
		if (fav1!=NULL) 
		{
			fpos_t ft1 = 0;
			char favt[256];
			int res=fscanf(fav1,"%256[^\n]",favt);
			while ( res>0 && res!=EOF )
			{
				ft1+=(strlen(favt)+2);
				std::fsetpos(fav1,&ft1);
				String^ favstr = gcnew String((char*)favt);
				this->comboBox1->Items->Add(favstr);
				res=fscanf(fav1,"%256[^\n]",favt);
			}
			std::fclose(fav1);
			if (this->comboBox1->Items->Count > 0)
				this->comboBox1->Text = this->comboBox1->Items[this->comboBox1->Items->Count-1]->ToString();
		}

		FILE* favnt1;
		favnt1 = fopen("favnt.txt", "r");
		if (favnt1 != NULL)
		{
			fpos_t ft1 = 0;
			char favt[256];
			int res = fscanf(favnt1, "%256[^\n]", favt);
			while (res > 0 && res != EOF)
			{
				ft1 += (strlen(favt) + 2);
				std::fsetpos(favnt1, &ft1);
				String^ favstr = gcnew String((char*)favt);
				this->comboBox2->Items->Add(favstr);
				res = fscanf(favnt1, "%256[^\n]", favt);
			}
			std::fclose(favnt1);
			if (this->comboBox2->Items->Count > 0)
				this->comboBox2->Text = this->comboBox2->Items[this->comboBox2->Items->Count - 1]->ToString();
		}
				
	}
	catch (Exception ^e)
	{
		DWORD errn = GetLastError();
		MessageBox::Show(e->Message);
		MessageBox::Show(errn.ToString());
	}	

}

System::Void Form1::listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			g1 = this->pictureBox1->CreateGraphics();
			float w = this->pictureBox1->Width  +  dx*2;
			float h = this->pictureBox1->Height  +  dy*2;
			//g1->Clear(Color::WhiteSmoke);
			pen1 = gcnew Pen(Color::Black, 1);
		 }

System::Void Form1::pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (e->Button == System::Windows::Forms::MouseButtons::Middle)
			 {
				 xc = e->X - dx;
				 yc = e->Y - dy;
				 gscroll=1;
			 }
			 if (e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 this->contextMenuStrip1->Show(e->X + this->Left + pictureBox1->Left + 10, e->Y + this->Top + pictureBox1->Top + 30);
			 }
			 else
			 {
				 this->contextMenuStrip1->Close();
			 }
		 }
System::Void Form1::pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (e->Button == System::Windows::Forms::MouseButtons::Middle)
			 {
				 xc = (float)e->X;
				 yc = (float)e->Y;
				 this->label1->Text = Convert::ToString(dx);
				 this->label2->Text = Convert::ToString(dy);
				 gscroll=0;
			 }
		 }
System::Void Form1::pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
	if (!gscroll)
	{
		 long realx = (long)((float)100.0f*(e->X - this->pictureBox1->Width/2 - dx)/mas);
		 long realy = (long)((float)100.0f*(this->pictureBox1->Height/2 + dy - e->Y)/mas);
		 this->label4->Text = "x = " + Convert::ToString((float)realx / 100);
		 this->label5->Text = "y = " + Convert::ToString((float)realy / 100);

		 bool active=0;
		 actindex=0;
		 this->contextMenuStrip1->Items->Clear();

		 if (allacts!=NULL) for (int i=0; i<=nacts; i++)
		 if (allacts[i].num > 0) 
		 {
			 if ((float)realx/100 > allacts[i].zone.x1 && \
				(float)realx/100 < allacts[i].zone.x2 && \
				(float)realy/100 > allacts[i].zone.y1 && \
				(float)realy/100 < allacts[i].zone.y2) 
			 {
				 active=true; 
				 actindex=i;
				  String^ str1 = gcnew String(allacts[i].kks);
				 {toolTip1->AutoPopDelay =30000; toolTip1->SetToolTip(this->pictureBox1, str1); toolTip1->IsBalloon=false;}
				 i = nacts;
			 }	// Я навел на активный объект - ссылка!

		 }

		 
		 for (unsigned long i=1; i<=nOBJS; i++)
		 try {
				 if (allobjs[i].num > 0 && allobjs[i].num <= nOBJS) 
				 {
					 if ((float)realx/100 > allobjs[i].pos.x-5.0f && \
						(float)realx/100 < allobjs[i].pos.x+5.0f && \
						(float)realy/100 > allobjs[i].pos.y-5.0f && \
						(float)realy/100 < allobjs[i].pos.y+5.0f) 
					 {
						 String^ str1 = ""; 
						 bool newline=0;
						 String^ str3 = gcnew String(allobjs[i].type);
						 String^ nobi_type = gcnew String(allobjs[i].t[0].text);
						 String^ nobi_n = gcnew String(allobjs[i].t[1].text);
						 String^ nobi_ch = "1";
						 String^ awe_n = gcnew String(info.awenum);
						 awe_n = awe_n->Trim();
						 String^ bg_n = gcnew String(info.bgnum);
						 bg_n = bg_n->Trim();
						 String^ kks = gcnew String(info.kks);
						 kks = kks->Trim();
						 if (str3 == "ubi.v" || str3 == "ubi.vb" || str3 == "ubi.vk") // Я навел на активный объект, который можно вызвать в NOBI
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",ESGV";
						 }
						 else if (str3 == "ubi.m" || str3 == "ubi.mb" || str3 == "ubi.mk")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",ESGM";
						 }
						 else if (str3 == "ubi.s" || str3 == "ubi.sbb" || str3 == "ubi.sk")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",ESGS";
						 }
						 else if (str3 == "ubi.ba1" || str3 == "ubi.ba2")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[4].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",BA";
						 }
						 else if (str3 == "ubi.vw1" || str3 == "ubi.vw2" || str3 == "ubi.vw3")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[4].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",VW";
						 }
						 else if (str3 == "ubi.ite")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",ITE";
						 }
						 else if (str3 == "ubi.ivl")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",IVL";
						 }
						 else if (str3 == "ubi.kr")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[4].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",REK";
						 }
						 else if (str3 == "ubi.bg.swv")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[4].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",IBR";
						 }
						 else if (str3 == "ubi.sr")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[4].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",RES";
						 }
						 else if (str3 == "ubi.kom")
						 {
							 active=true;
							 nobi_ch = gcnew String(allobjs[i].t[3].text);
							 this->contextMenuStrip1->Items->Add("NOBI,"+nobi_type+","+nobi_n+"("+str3+")" );
							 this->contextMenuStrip1->Items[this->contextMenuStrip1->Items->Count-1]->ToolTipText = awe_n+","+bg_n+","+nobi_ch+","+kks+",KOM";
						 }
						 for (int q1=0; q1<16; q1++)
						 if (allobjs[i].t[q1].text[0] != '\0')
						 {
							 String^ str2 = gcnew String(allobjs[i].t[q1].text);
							 if (!String::IsNullOrEmpty(str2))
							 {
								 str1=str1+str2+"  \t";
								 if ((q1==3) && (str2->StartsWith("M") || str2->StartsWith("E") || str2->StartsWith("T") || str2->StartsWith("A"))  )
								 {
									 active=true;
									 int sp = str2->IndexOf(" ");
									 if (sp>0) this->contextMenuStrip1->Items->Add(str2->Substring(0,sp));
									 else this->contextMenuStrip1->Items->Add(str2);
								 }
							 }
						 }
						 str1=str1+"\n";
						 for (unsigned long q1=1; q1<=nINPS; q1++)
						 if (allinps[q1].parent == i)
						 {
							for (int q2=0; q2<8; q2++)
							if (allinps[q1].t[q2].vis != 1) 
							{
								 active=true;
								 newline=true;
							}
							if (newline) for (int q2=1; q2<6; q2++)
							if (allinps[q1].t[q2].text[0]!='\0')
							{
								 String^ str2 = gcnew String(allinps[q1].t[q2].text);
								 if (!String::IsNullOrEmpty(str2))
								 {
									 str1=str1+str2+"  \t";
									 if ((q2==3) && (str2->StartsWith("M") || str2->StartsWith("E") || str2->StartsWith("T") || str2->StartsWith("A")))
									 {
										 int sp = str2->IndexOf(" ");
										 if (sp>0) this->contextMenuStrip1->Items->Add(str2->Substring(0,sp));
										 else this->contextMenuStrip1->Items->Add(str2);
									 }
								 }
							}
							if (newline) {str1=str1+"\n"; newline=0;}
						 }
						 //if ()
							if (!oldactive && str1!="" && this->tipsToolStripMenuItem->CheckState == CheckState::Checked) {toolTip1->AutoPopDelay =30000; toolTip1->SetToolTip(this->pictureBox1, str1); toolTip1->IsBalloon=false; oldactive=true;}
						 i = nOBJS;
					 }	// Я навел на ubi!
				 }
			 } 
			 catch (Exception^ e) 
			 {
				DWORD err = GetLastError( );
				this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
			 }

		 //if (this->tipsToolStripMenuItem->CheckState == CheckState::Checked)
		 for (unsigned long i=1; i<=nTABS; i++)
		 if (alltabs[i].num > 0 && alltabs[i].prop[6].text[0]!='\0') 
		 {
			 if ((float)realx/100 > alltabs[i].prop[6].x && \
				(float)realx/100 < alltabs[i].prop[6].x+13.0f && \
				(float)realy/100 > alltabs[i].prop[6].y && \
				(float)realy/100 < alltabs[i].prop[6].y+3.0f) 
			 {
				 active=true;
				 String^ str2 = gcnew String(alltabs[i].prop[6].text);
				 this->contextMenuStrip1->Items->Add(str2);
				 if (!oldactive && str2!="") {toolTip1->AutoPopDelay = 30000; toolTip1->SetToolTip(this->pictureBox1, str2); toolTip1->IsBalloon =false; oldactive=true;}
				 i = nTABS;
			 }	// Я навел на маркер в таблице!

		 }

		 if (!active || actindex>0 ) oldactive=false;

		 //if (!active) {toolTip1->SetToolTip(this->pictureBox1, ""); }

		 if (active || oldactive) this->pictureBox1->Cursor = Cursors::Hand;
		 else this->pictureBox1->Cursor = Cursors::Cross;

		 if (!this->FindBox->Focused) this->pictureBox1->Focus();
	 }
	 else
	 {
		 dx = e->X - xc;
		 dy = e->Y - yc;
		 this->label1->Text = Convert::ToString(dx);
		 this->label2->Text = Convert::ToString(dy);
		 if (this->SearchResults->SelectedIndex >=0) this->SearchResults_SelectedIndexChanged(sender,e);
	 }
}
System::Void Form1::pictureBox1_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (e->Button == System::Windows::Forms::MouseButtons::Middle)
			 {
				 if (tabControl1->SelectedIndex == 0)
				 {
					 if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 420.0f / 297.0f) mas = this->pictureBox1->Width / 420.0f;
					 else mas = this->pictureBox1->Height / 297.0f;
					 dx = -210.0f*mas; dy = 148.5f*mas;
					 font1 = gcnew System::Drawing::Font("MS Sans Serif", 2.0f*mas);
				 }
				 else if (tabControl1->SelectedIndex == 1)
				 {
					 if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 1112.0f / 782.0f) mas = this->pictureBox1->Width / 1112.0f;
					 else mas = this->pictureBox1->Height / 782.0f;
					 dx = -556.0f*mas; dy = 391.0f*mas;
					 font1 = gcnew System::Drawing::Font("MS Sans Serif", 6.0f*mas);
				 }
				 this->label1->Text = Convert::ToString(dx);
				 this->label2->Text = Convert::ToString(dy);
				 this->label3->Text = Convert::ToString(mas);
				 if (this->SearchResults->SelectedIndex >=0) this->SearchResults_SelectedIndexChanged(sender,e);
			 }
			 else if (e->Button == System::Windows::Forms::MouseButtons::Left)
			 {
				if (this->timer2->Enabled && this->contextMenuStrip1->Items->Count > 0)
				{
					Windows::Forms::ToolStripItemClickedEventArgs^ cleck_e;
					this->contextMenuStrip1_ItemClicked("DoubleClick",cleck_e);
				}

			 }
		 }

System::Void Form1::pictureBox1_MouseWheel(Object^ sender, System::Windows::Forms::MouseEventArgs^ e) 
		 {
			 if (e->Delta > 0) 
			 {
				 float realx = (e->X - this->pictureBox1->Width/2 - dx)/mas;
				 float realy = (this->pictureBox1->Height/2 + dy - e->Y)/mas;
				 mas*=1.21f;
				 dx = e->X - realx*mas - this->pictureBox1->Width/2;
				 dy = e->Y + realy*mas - this->pictureBox1->Height/2;
			 }
			 else if (e->Delta < 0)
			 {
				 float realx = (e->X - this->pictureBox1->Width/2 - dx)/mas;
				 float realy = (this->pictureBox1->Height/2 + dy - e->Y)/mas;
				 mas/=1.21f;
				 dx = e->X - realx*mas - this->pictureBox1->Width/2;
				 dy = e->Y + realy*mas - this->pictureBox1->Height/2;
			 }

			 if (tabControl1->SelectedIndex == 0)
			 {
				 font1 = gcnew System::Drawing::Font("MS Sans Serif", 2.0f*mas);
			 }
			 else if (tabControl1->SelectedIndex == 1)
			 {
				 font1 = gcnew System::Drawing::Font("MS Sans Serif", 6.0f*mas);
			 }

			 this->label1->Text = Convert::ToString(e->X);
		     this->label2->Text = Convert::ToString(e->Y);
			 this->label3->Text = Convert::ToString(mas);
			 if (this->SearchResults->SelectedIndex >=0) this->SearchResults_SelectedIndexChanged(sender,e);
		 }

System::Void Form1::button3_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->slist.Clear();
			this->treeView1->Nodes->Clear();
			if (tnc != nullptr) tnc->Clear();
			this->PlayButton->Enabled = false;
			this->StopButton->Enabled = false;
			this->PauseButton->Enabled = false;

			SearchResults->Items->Clear();
			nobi->Clear();
			

		    String^ str0 = this->comboBox1->Text;
		    WIN32_FIND_DATAA FindFileData;
			std::memset(&FindFileData, sizeof(WIN32_FIND_DATA ),0);
		
			char *v = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str0+"\\sicherung\\a.*").ToPointer();

			HANDLE hf;
			hf=FindFirstFileA((LPCSTR)v, (LPWIN32_FIND_DATAA) &FindFileData);
			if (hf!=INVALID_HANDLE_VALUE)
			{
				FILE* favout=fopen("fav.txt","w");
				bool dup=false;
				for (int i=0; i<this->comboBox1->Items->Count; i++) 
				if (this->comboBox1->Items[i]->ToString()==str0) 
				{
					dup=true;
					this->comboBox1->Items->RemoveAt(i);
					this->comboBox1->Items->Add(str0);
					i=this->comboBox1->Items->Count+1;
				}
				if (dup==false) 
				{				
					this->comboBox1->Items->Add(str0);
				}
				for (int i=0; i<this->comboBox1->Items->Count; i++)
				{
					char *v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->comboBox1->Items[i]->ToString()).ToPointer();
					fprintf(favout,"%s\n",v2);
				}
				this->comboBox1->Text = str0;
				std::fclose(favout);

				String^ kks = ""; 
				long curnode=0;
				//this->treeView1->Nodes->Clear();
				do	{
					String^ str1 = gcnew String(FindFileData.cFileName);

					// Собираю общие таблицы ab_q_rel и ab_z_rel .... и т. д.
					String^ ea_addr = str0+"\\sicherung\\"+str1+"\\ea.*";
					char *v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ea_addr).ToPointer();
					HANDLE hf2;
					hf2=FindFirstFileA((LPCSTR)v2, (LPWIN32_FIND_DATAA) &FindFileData);
					if (hf2!=INVALID_HANDLE_VALUE)
					{
						String^ ea_name = gcnew String(FindFileData.cFileName);
						this->treeView1->Nodes->Add(str1->Substring(2)+"("+ea_name->Substring(3)+")"); // создаю узел с именем шкафа
						this->toolStripLabel1->Text = "Обнаружено стоек: " + Convert::ToString(this->treeView1->Nodes->Count);
						this->Refresh();

						String^ bg_addr = str0+"\\sicherung\\"+str1+"\\"+ea_name+"\\bg.*";
						char *v3 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(bg_addr).ToPointer();
						HANDLE hf3;
						hf3=FindFirstFileA((LPCSTR)v3, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf3!=INVALID_HANDLE_VALUE)
						{
							do	{
									String^ bg_name = gcnew String(FindFileData.cFileName);
									this->treeView1->Nodes[curnode]->Nodes->Add(bg_name->Substring(3)); // создаю узел в пределах шкафа с номером модуля
									
									// ищу код добавляемого в дерево модуля
									String^ code_addr = str0+"\\sicherung\\"+str1+"\\"+ea_name+"\\fup.cod\\"+bg_name->Substring(3)+".bgp";
									char *v4 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(code_addr).ToPointer();
									HANDLE hf4;
									hf4=FindFirstFileA((LPCSTR)v4, (LPWIN32_FIND_DATAA) &FindFileData);
									if (hf4!=INVALID_HANDLE_VALUE) 
									{
										FILE* in_code=fopen(v4,"r");
										if (in_code!=NULL)
										{
											char line[128*1024];
											
											while( !feof( in_code ) )
											{
												std::memset(line,0,128*1024);
												int size = std::fread(line,1,128*1024,in_code);
												if (size>0 && size<128*1024)
												{
													line[size] = '\0';
													String^ code_s = gcnew String(line);
													this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count - 1]->ToolTipText += code_s;
												}
											};
											std::fclose(in_code);
										}
										// Разбор кода модуля на элементарные команды будет при первом выборе шкафов на эмуляцию
									}
									FindClose(hf4);

									// Добавляю в дерево все схемы модуля
									String^ list_addr = str0+"\\sicherung\\"+str1+"\\"+ea_name+"\\"+bg_name+"\\fup.log\\*.*";
									char *v5 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(list_addr).ToPointer();
									HANDLE hf5;
									hf5=FindFirstFileA((LPCSTR)v5, (LPWIN32_FIND_DATAA) &FindFileData);
									if (hf5!=INVALID_HANDLE_VALUE) 
									{
										do	{
											String^ list_name = gcnew String(FindFileData.cFileName);
											int res1 = list_name->IndexOf("#");
											if (res1>0 && list_name->Substring(res1)->Contains("#"))
											{
												int res2 = list_name->Substring(res1+1)->IndexOf("#");
												if (res2>0) 
												{
													int res3 = list_name->Substring(res1+res2+2)->IndexOf("#");

													bool aexist=false;
													int list_count = this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count-1]->Nodes->Count;
													for (int ki=0; ki<list_count; ki++)
														if (res3>0 && this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count-1]->Nodes[ki]->Text == list_name->Substring(res1+2,res2-1))
														{
															aexist=true;
															this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count-1]->Nodes[ki]->ToolTipText += list_name->Substring(res1+res2+1,res3+1);
														}
														if (aexist) continue;
														this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count-1]->Nodes->Add(list_name->Substring(res1+2,res2-1));
														this->treeView1->Nodes[curnode]->Nodes[this->treeView1->Nodes[curnode]->Nodes->Count-1]->Nodes[list_count]->ToolTipText = list_name->Substring(res1+res2+1,res3+1);
												}
											}
										} while (FindNextFileA(hf5,&FindFileData)!=0);
										
									}
									FindClose(hf5);
							
							} while (FindNextFileA(hf3,&FindFileData)!=0);
							FindClose(hf3);
						}
					}
					
					
					if (str1->Length > 3)
					{

						String^ addr = str0+"\\sicherung\\"+str1+"\\fup.gra\\yf\\";
						char *v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(addr+"yf#=*").ToPointer();
						HANDLE hf2;
						hf2=FindFirstFileA((LPCSTR)v2, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf2!=INVALID_HANDLE_VALUE)
						{
							do	{
								String^ str2 = gcnew String(FindFileData.cFileName);
								if (str2->Length > 3 && FindFileData.nFileSizeLow > 500) // заполняю содержимое узла
								{
									int kksend = str2->IndexOf('#',4);
									int listend = str2->IndexOf('#',kksend+1);
									this->slist.Add(addr+str2,str2->Substring(4,listend-4));
								}
							} while (FindNextFileA(hf2,&FindFileData)!=0);
						}

						addr = str0+"\\sicherung\\"+str1+"\\fup.gra\\ym\\";
						v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(addr+"ym#=*").ToPointer();
						hf2=FindFirstFileA((LPCSTR)v2, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf2!=INVALID_HANDLE_VALUE)
						{
							do	{
								String^ str2 = gcnew String(FindFileData.cFileName);
								if (str2->Length > 3 && FindFileData.nFileSizeLow > 500) // заполняю содержимое узла
								{
									int kksend = str2->IndexOf('#',4);
									int listend = str2->IndexOf('#',kksend+1);
									this->slist.Add(addr+str2,str2->Substring(4,listend-4));
								}
							} while (FindNextFileA(hf2,&FindFileData)!=0);
						}
						
						addr = str0+"\\sicherung\\"+str1+"\\fup.gra.akt\\yf\\";
						//HANDLE hf2;
						v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(addr+"yf#=*").ToPointer();
						hf2=FindFirstFileA((LPCSTR)v2, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf2!=INVALID_HANDLE_VALUE)
						{
							do	{
								String^ str2 = gcnew String(FindFileData.cFileName);
								if (str2->Length > 3 && FindFileData.nFileSizeLow > 500) // заполняю содержимое узла
								{
									int kksend = str2->IndexOf('#',4);
									int listend = str2->IndexOf('#',kksend+1);
									if (slist.IndexOfValue(str2->Substring(4,listend-4))<0 )
										this->slist.Add(addr+str2,str2->Substring(4,listend-4));
								}
							} while (FindNextFileA(hf2,&FindFileData)!=0);
						}

						addr = str0+"\\sicherung\\"+str1+"\\fup.gra.akt\\ym\\";
						v2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(addr+"ym#=*").ToPointer();
						hf2=FindFirstFileA((LPCSTR)v2, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf2!=INVALID_HANDLE_VALUE)
						{
							do	{
								String^ str2 = gcnew String(FindFileData.cFileName);
								if (str2->Length > 3 && FindFileData.nFileSizeLow > 500) // заполняю содержимое узла
								{
									int kksend = str2->IndexOf('#',4);
									int listend = str2->IndexOf('#',kksend+1);
									if (slist.IndexOfValue(str2->Substring(4,listend-4))<0 )
										this->slist.Add(addr+str2,str2->Substring(4,listend-4));
								}
							} while (FindNextFileA(hf2,&FindFileData)!=0);
						}

						FindClose(hf2);
						
					}
					
					curnode++;
				} while (FindNextFileA(hf,&FindFileData)!=0);			
					
				FindClose(hf);
				
				this->treeView1->Sort();	
				this->PlayButton->Enabled = (this->treeView1->Nodes->Count > 0);

			};
		 }
System::Void Form1::SearchResults_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {

	if (tabControl1->SelectedIndex == 1)
	{
		//w = this->pictureBox1->Width + dx * 2 + mas * xc * 2;
		//h = this->pictureBox1->Height + dy * 2 - mas * yc * 2;
		try
		{
			this->listBox1->SelectedIndex = -1;
			if (this->SearchResults->SelectedIndex >= 0 && this->SearchResults->SelectedItem->ToString()->Length > 0)
			{
				newindex = this->slint.IndexOfValue(this->SearchResults->SelectedItem->ToString());
				w = this->pictureBox1->Width + dx * 2;
				h = this->pictureBox1->Height + dy * 2;
				g1->Clear(*myforecolor);
				pen1->Width = mas * 0.35f;
				pen2->Width = mas * 0.35f;
			}
			else newindex = -1;

			// первый проход для сбора объектов
			if (newindex != oldindex && newindex >= 0)
			{
				if (!undoredo)
				{
					if (histpos < 1) this->UndoButton->DropDownItems->Add(this->SearchResults->SelectedItem->ToString());
					else
					{
						this->UndoButton->DropDownItems->Add(this->SearchResults->SelectedItem->ToString());
						for (int q1 = this->UndoButton->DropDownItems->Count - 1; q1 > histpos + 1; q1--)
						{
							this->UndoButton->DropDownItems[q1]->Text = this->UndoButton->DropDownItems[q1 - 1]->Text;
						}
						this->UndoButton->DropDownItems[histpos + 1]->Text = this->SearchResults->SelectedItem->ToString();
					}
					if (histpos >= 0) this->UndoButton->DropDownItems[histpos]->BackColor = SystemColors::Control;
					histpos++;
					if (histpos > 25)
					{
						this->UndoButton->DropDownItems->RemoveAt(0);
						histpos--;
					}
				}
				else undoredo = false;
				if (histpos >= 0) this->UndoButton->DropDownItems[histpos]->BackColor = Color::DarkGray;

				if (allpuns != NULL) { std::memset(&allpuns, 0, sizeof(allpuns)); delete allpuns; nPUNS = 0; }
				if (punsadd != NULL) { std::memset(&punsadd, 0, sizeof(punsadd)); delete punsadd; }
				if (alllins != NULL) { std::memset(&alllins, 0, sizeof(alllins)); delete alllins; nLINS = 0; }
				if (linsadd != NULL) { std::memset(&linsadd, 0, sizeof(linsadd)); delete linsadd; }
				if (allcons != NULL) { std::memset(&allcons, 0, sizeof(allcons)); delete allcons; nCONS = 0; }
				if (alltabs != NULL) { std::memset(&alltabs, 0, sizeof(alltabs)); delete alltabs; nTABS = 0; }
				if (allobjs != NULL) { std::memset(&allobjs, 0, sizeof(allobjs)); delete allobjs; nOBJS = 0; }
				if (allacts != NULL) { std::memset(&allacts, 0, sizeof(allacts)); delete allacts; nINPS = 0; }

				std::memset(&info, 0, sizeof(info));

				String^ schemaget_s = "";
				String^ abonent_id_s = "";
				String^ page_id_s = "";
				if (slint.Keys[newindex]->Contains(".") && this->slint.Keys[newindex]->Contains(":"))
				{
					schemaget_s = slint.Keys[newindex]->Substring(0, slint.Keys[newindex]->IndexOf("."));
					abonent_id_s = slint.Keys[newindex]->Substring(slint.Keys[newindex]->IndexOf(".") + 1, slint.Keys[newindex]->IndexOf(":") - slint.Keys[newindex]->IndexOf(".") - 1);
					page_id_s = slint.Keys[newindex]->Substring(slint.Keys[newindex]->IndexOf(":") + 1);
				}
				if (schemaget_s != "" && abonent_id_s != "" && page_id_s != "")
				{
					int abonent_id = Convert::ToInt32(abonent_id_s);
					int page_id = Convert::ToInt32(page_id_s);
					char* complex = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(schemaget_s).ToPointer();
					Params p; p.add((int)abonent_id); p.add((int)page_id);

					auto streams = base->getBlocksByteArrayStreams(complex, &p);
					//collector = &BlockCollector::Instance();
					collector->clearFblocks();

					auto bl = base->getBlocks(complex, &p);
					auto br = base->getBorder(complex, &p);
					auto bp = base->getBlockParams(complex, &p);
					auto ap = base->getAddedPorts(complex, &p);
					auto rp = base->getRaisedPorts(complex, &p);
					auto cn = base->getConnectors(complex, &p);
					auto fc = base->getFplConnectors(complex, &p);
					auto ip = base->getBlocksWithInvertPorts(complex, &p);

					for (auto it = streams->begin(); it != streams->end(); it++)
					{
						auto block = collector->get(it->first);
						if (block == nullptr) {
							auto b = new GroupBlockObj();
							b->read(it->second);
							b->groupObj();
							collector->put(b);
						}
					}

					nTABS = br.size();
					if (nTABS < 0 || nTABS > 100) nTABS = 100;
					alltabs = new TABS[nTABS + 1];
					std::memset(alltabs, 0, sizeof(TABS)*(nTABS + 1));
					this->listBox2->Items->Add("nTABS=" + Convert::ToString((long)nTABS));
					int tabn = 0;

					allacts = new ACTS[nTABS + 1]; // + количество портов!!!
					std::memset(allacts, 0, sizeof(ACTS)*(nTABS + 1));
					this->listBox2->Items->Add("nACTS=" + Convert::ToString((long)nTABS));
					int actn = 0;

					nCONS = fc.size();
					if (nCONS < 0 || nCONS > 500) nCONS = 500;
					allcons = new CONS[nCONS + 1];
					std::memset(allcons, 0, sizeof(TABS)*(nCONS + 1));
					this->listBox2->Items->Add("nCONS=" + Convert::ToString((long)nCONS));
					int conn = 0;

					string tabelem[10] = { "adr1","adr2","fkz","plhw","sah","sig","verw","znra","bez","eins"};

					//int i = 0;
					for (auto it = bl.begin(); it != bl.end(); it++)
					{
						auto currentBlock = it->second;
						auto group = collector->get(currentBlock.fields.at("block_name"));
						if (group != nullptr)
						{
							int idx = strToInt(currentBlock.fields.at("block_id"), 10);
							GroupBlockObj* fblock = new GroupBlockObj(*group);
							if (ap.find(currentBlock.fields.at("block_id")) != ap.end())
							{
								fblock->setPortsAdded(true);
								int count = strToInt(ap.at(currentBlock.fields.at("block_id")).fields.at("count"), 10);
								fblock->setCountAddedPort(count);
							}
							if (rp.find(currentBlock.fields.at("block_id")) != rp.end())
							{
								fblock->setPortsRaised(true);
								int count = strToInt(rp.at(currentBlock.fields.at("block_id")).fields.at("count"), 10);
								fblock->setCountRaisedPort(count);
							}
							collector->put(idx, fblock);
						}
					}

					auto pageStream = base->getByteArrayStream(complex, &p);
					page = new Page;
					page->read(pageStream);
					LayerList* ll = page->getLayerList();
					for (int ki = 0; ki < ll->getCount(); ki++)
					{
						LayerObj* lo = ll->get(ki);
						unsigned int gc = lo->objectList_->getCount();
						nOBJS += gc;
						if (lo != nullptr /*&& lo->getName().compare("page.userlayer.name") == 0*/)
						{
							for (int j = 0; j < lo->objectList_->objs_->size(); j++)
							{
								if (lo->objectList_->get(j)->getLineFormat()->lineWidth_ > 0)
								{
									nLINS += (lo->objectList_->get(j)->getNodeList()->getCount() - 1);
								}
								if (lo->objectList_->get(j)->getPortCount() > 0)
								{
									nINPS += (lo->objectList_->get(j)->getPortCount());
								}
							}
						}
					}

					if (nOBJS < 0 || nOBJS > 100) nOBJS = 100;
					allobjs = new OBJS[nOBJS + 1];
					std::memset(allobjs, 0, sizeof(OBJS)*(nOBJS + 1));
					this->listBox2->Items->Add("nOBJS=" + Convert::ToString((long)nOBJS));

					if (nLINS < 0 || nLINS>300) nLINS = 300;
					alllins = new LINS[nLINS + 1];
					linsadd = new unsigned long[nLINS + 1];
					std::memset(alllins, 0, sizeof(LINS)*(nLINS + 1));
					std::memset(linsadd, 0, 4 * (nLINS + 1));
					this->listBox2->Items->Add("nLINS=" + Convert::ToString((long)nLINS));
					int linn = 0;

					if (nINPS < 0 || nINPS>500) nINPS = 500;
					allinps = new INPS[nINPS + 1];
					std::memset(allinps, 0, sizeof(INPS)*(nINPS + 1));
					this->listBox2->Items->Add("nINPS=" + Convert::ToString((long)nINPS));
					int inpn = 0;

					int i = 0;
					for (int ki = 0; ki < ll->getCount(); ki++)
					{
						LayerObj* lo = ll->get(ki);
						if (lo != nullptr)
						{
							for (int j = 0; j < lo->objectList_->objs_->size(); j++)
							{
								BasicObj* ob = lo->objectList_->get(j);
								string bn = ob->getName();
								String^ bn_s = gcnew String(ob->getName().c_str());
								LineFormat* lf = ob->getLineFormat();
								NodeList* nl = ob->getNodeList();
								PortList* pl = ob->getPortList();
								unsigned int type = ob->getType();
								int idx = -1;
								GroupBlockObj* gbo;
								SimplePoint* sp;
								SimplePoint* offset;

								if (bn_s != "") //это составной объект
								{
									gbo = (GroupBlockObj*)ob;
									sp = gbo->getPos();
									offset = gbo->offset_;
									idx = gbo->getBlockId();
									if (bn == "FunctionBorder" || bn == "MeasureBorder" || bn == "FunctionBorderEM" || bn == "MeasureBorderEM") idx = 0;
									String^ x_s = "0.0";
									String^ y_s = "0.0";
									if (bl.find(intToStr(idx, 10)) != bl.end())
									{
										x_s = gcnew String(bl.at(intToStr(idx, 10)).fields.at("xpos").c_str());
										y_s = gcnew String(bl.at(intToStr(idx, 10)).fields.at("ypos").c_str());
									}
									else
									{
										x_s = Convert::ToString(offset->X());
										y_s = Convert::ToString(offset->Y());
									}

									i++;
									allobjs[i].num = idx + 1000;
									strcpy(allobjs[i].type, bn.c_str());
									if (bn == "FunctionBorder" || bn == "MeasureBorder" || bn == "FunctionBorderEM" || bn == "MeasureBorderEM")
									{
										allobjs[i].pos.x = Convert::ToSingle(x_s)+0.7f;
										allobjs[i].pos.y = Convert::ToSingle(y_s)+0.35f;
									}
									else if (bn_s->Contains("rahmen"))
									{
										allobjs[i].pos.x = Convert::ToSingle(x_s);
										allobjs[i].pos.y = Convert::ToSingle(y_s);
									}
									else
									{
										allobjs[i].pos.x = sp->X();
										allobjs[i].pos.y = sp->Y();
									}
									allobjs[i].color = 0;
									float x1 = allobjs[i].pos.x;
									float y1 = allobjs[i].pos.y;

									int nump = 0;
									bool nextconn = true;

									for (int fbe = 0; fbe < gbo->objectList_->getCount(); fbe++)
									{
										auto bo = gbo->objectList_->get(fbe);
										if (bo != nullptr)
										{
											unsigned int type = bo->getType();
											unsigned int align = bo->getTextAlingment();
											
											string tv = bo->getTextValue();
											String^ tv_s_new = "";
											if (tv != "")
											{
												wchar_t* wstr = toUTF8(tv);
												//tv_s_new = gcnew String(tv.c_str()); //
												tv_s_new = gcnew String(wstr);
												//bo->setTextValue(strTostr(tv_s_new));
											}

											string en = bo->getName();
											String^ en_s = gcnew String(bo->getName().c_str());
											LineFormat* lf = bo->getLineFormat();

											if ((type == 200 || type == 300) && bn_s->Contains("plk")) // 200 - TabelObj , 300 - TextObj
											{
												SimplePoint* sp1 = bo->getNodeList()->get(0)->getPos();
												SimplePoint* sp2 = bo->getNodeList()->get(1)->getPos();

												for (auto it2 = fc.begin(); it2 != fc.end(); it2++)
													if (strToInt(it2->fields.at("block_id"), 10) == idx)
													{
														if (nextconn && conn < nCONS)
														{
															conn++;
															allcons[conn].num = conn;
															nextconn = false;
														}
														if (lf->lineWidth_ > 0)
														{
															allcons[conn].pos.x = x1 - offset->X() + sp1->X();
															allcons[conn].pos.y = y1 - offset->Y() + sp1->Y();
														}
														else if (en == "delimiter")
														{
															x1 = allobjs[i].pos.x - offset->X() + sp2->X();
															y1 = allobjs[i].pos.y - offset->Y() + sp2->Y();
														}
														else if (en == "name")
														{
															String^ conname_s = gcnew String(it2->fields.at("name").c_str());
															x1 -= offset->X();
															y1 -= offset->Y();
															if (align == 0) x1 += (float)sp1->X();
															else if (align == 1) x1 += ((float)sp1->X() + (float)sp2->X()) / 2.0f - conname_s->Length * 3.0f;
															else x1 += (float)sp2->X() - conname_s->Length * 6.2f;
															y1 += sp1->Y();
															allcons[conn].conname.vis = 1;
															allcons[conn].conname.x = x1;
															allcons[conn].conname.y = y1 + 1.0f;
															strcpy(allcons[conn].conname.text, it2->fields.at("name").c_str());
															strcpy(allcons[conn].slashtype, bn.c_str());
														}
														else if (en == "page_label")
														{
															String^ conlabel_s = gcnew String(it2->fields.at("label").c_str());
															int res = this->step_splitter(conlabel_s, splitlist);
															if (res > 0 && splitlist->Count > 0)
															{
																for (int nc = 0; nc < splitlist->Count; nc++)
																	if (nc <= 9)
																	{
																		allcons[conn].cons[nc].vis = 1;
																		allcons[conn].cons[nc].x = x1;
																		allcons[conn].cons[nc].y = y1 + nc * 11.0f + 1.0f;
																		strcpy(allcons[conn].cons[nc].text, strTostr(splitlist[nc]).c_str());
																	}
															}
														}
													}
											}
											if (bn != "FunctionBorder" && bn != "MeasureBorder" && bn != "FunctionBorderEM" && bn != "MeasureBorderEM")
												for (int q = 0; q < bp.size(); q++)
												{
													if (strToInt(bp.at(q).fields.at("block_id"), 10) == idx && bo->getName() == bp.at(q).fields.at("text_value") && nump < 16)
													{
														String^ tog2 = gcnew String(bp.at(q).fields.at("graph_value").c_str());
														int len = tog2->Length;
														if (len > 0 && len < 21);
														{
															//SimplePoint* sp = bo->textValue_->getPos();
															SimplePoint* sp1 = bo->getNodeList()->get(0)->getPos();
															SimplePoint* sp2 = bo->getNodeList()->get(1)->getPos();

															pen1->Color = Color::Green;
															NodeList* nl2 = bo->getNodeList();
															for (int k = 0; k < nl->getCount()-1; k++)
															{
																pen1->Width = mas * 0.35f * (float)lf->lineWidth_;
																SimplePoint* ssp1 = nl2->get(k)->getPos();
																SimplePoint* ssp2 = nl2->get(k + 1)->getPos();
																float x1 = (float)sp->X() + (float)ssp1->X();
																float y1 = (float)sp->Y() + (float)ssp1->Y(); y1 = -y1;
																float x2 = (float)sp->X() + (float)ssp2->X();
																float y2 = (float)sp->Y() + (float)ssp2->Y();; y2 = -y2;
																g1->DrawLine(pen1, w / 2 + mas * x1, h / 2 - mas * y1, w / 2 + mas * x2, h / 2 - mas * y2);
															}
															pen1->Color = Color::Black;

															//float x1 = allobjs[i].pos.x ;
															x1 = allobjs[i].pos.x;
															y1 = allobjs[i].pos.y + (float)sp2->Y() + 1.0f;
															if (align == 0) x1 += (float)Math::Min(sp1->X(), sp2->X());
															else if (align == 1) x1 += ((float)sp1->X() + (float)sp2->X()) / 2.0f - tog2->Length * 2.0f;
															else x1 += (float)Math::Max(sp1->X(), sp2->X()) - tog2->Length * 4.0f;
															allobjs[i].t[nump].vis = 1;
															allobjs[i].t[nump].x = x1;
															allobjs[i].t[nump].y = y1;
															strcpy(allobjs[i].t[nump].text, bp.at(q).fields.at("graph_value").c_str());
															allobjs[i].t[nump].text[len] = '\0';
															nump++;
														}
													}
												}
											else if ((bn == "FunctionBorder" || bn == "MeasureBorder" || bn == "FunctionBorderEM" || bn == "MeasureBorderEM") && (en_s->Contains("vis_L_") || en_s->Contains("vis_R_")))
											{
												int cur = 0;
												SimplePoint* sp1 = bo->getNodeList()->get(0)->getPos();
												SimplePoint* sp2 = bo->getNodeList()->get(1)->getPos();
												SimplePoint* sp3 = bo->getNodeList()->get(2)->getPos();
												SimplePoint* sp4 = bo->getNodeList()->get(3)->getPos();
												for (auto it2 = br.begin(); it2 != br.end(); it2++)
												{
													cur++;
													string pn = it2->fields.at("port_name");
													for (int j = 0; j < 10; j++)
														if (pn + tabelem[j] == en)
														{
															alltabs[cur].color = 0;
															alltabs[cur].prop[j].x = allobjs[i].pos.x - offset->X() + (float)sp3->X();
															alltabs[cur].prop[j].y = allobjs[i].pos.y - offset->Y() + (float)sp3->Y() - 4.1f;
															alltabs[cur].prop[j].text[0] = '\0';
															if (it2->fields.at(tabelem[j]).length() > 0)
															{
																alltabs[cur].num = cur;
																alltabs[cur].prop[j].vis = 1;
																if (tabelem[j].compare("bez") == 0 || tabelem[j].compare("eins") == 0)
																{
																	string tv2 = it2->fields.at(tabelem[j]);
																	String^ tv_s_new2 = "";
																	wchar_t* wstr2 = toUTF8(tv2);
																	tv_s_new2 = gcnew String(wstr2);
																	char *vt = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tv_s_new2).ToPointer();
																	strcpy(alltabs[cur].prop[j].text, vt);
																}
																else
																{
																	strcpy(alltabs[cur].prop[j].text, it2->fields.at(tabelem[j]).c_str());
																}
																if (tabelem[j].compare("znra") == 0)
																{
																	allacts[cur].num = cur;
																	allacts[cur].zone.x1 = alltabs[cur].prop[j].x;
																	allacts[cur].zone.y1 = alltabs[cur].prop[j].x;
																	allacts[cur].zone.x2 = alltabs[cur].prop[j].x + it2->fields.at(tabelem[j]).length() * 4.1f;
																	allacts[cur].zone.y2 = alltabs[cur].prop[j].x + 12.0f;
																	strcpy(allacts[cur].list, it2->fields.at("znra").c_str());
																	strcpy(allacts[cur].kks, it2->fields.at("plhw").c_str());
																}
															}
														}
												}
											}
											if ((en == "vis_prgt" || en == "vis_bgf") && nump < 16)
											{
												SimplePoint* sp1 = bo->getNodeList()->get(0)->getPos();
												SimplePoint* sp2 = bo->getNodeList()->get(1)->getPos();
												String^ tog2 = "";
												if (bl.find(intToStr(idx, 10)) != bl.end())
												{
													tog2 = gcnew String(bl.at(intToStr(idx, 10)).fields.at(en).c_str());
												}

												pen1->Color = Color::Magenta;
												NodeList* nl2 = bo->getNodeList();
												for (int k = 0; k < nl->getCount() - 1; k++)
												{
													pen1->Width = mas * 0.35f * (float)lf->lineWidth_;
													SimplePoint* ssp1 = nl2->get(k)->getPos();
													SimplePoint* ssp2 = nl2->get(k + 1)->getPos();
													float x1 = allobjs[i].pos.x + (float)ssp1->X();
													float y1 = allobjs[i].pos.y + (float)ssp1->Y(); y1 = -y1;
													float x2 = allobjs[i].pos.x + (float)ssp2->X();
													float y2 = allobjs[i].pos.y + (float)ssp2->Y();; y2 = -y2;
													g1->DrawLine(pen1, w / 2 + mas * x1, h / 2 - mas * y1, w / 2 + mas * x2, h / 2 - mas * y2);
												}
												pen1->Color = Color::Black;

												x1 = allobjs[i].pos.x;
												y1 = allobjs[i].pos.y + (float)sp2->Y() + 1.0f;
												if (align == 0) x1 += (float)Math::Min(sp1->X(), sp2->X());
												else if (align == 1) x1 += ((float)sp1->X() + (float)sp2->X()) / 2.0f - tog2->Length * 3.0f;
												else x1 += (float)Math::Max(sp1->X(), sp2->X()) - tog2->Length * 6.2f;
												allobjs[i].t[nump].vis = 1;
												allobjs[i].t[nump].x = x1;
												allobjs[i].t[nump].y = y1;
												allobjs[i].t[nump].text[0] = '\0';
												if (tog2 != "" && tog2->Length < 21)
												{
													strcpy(allobjs[i].t[nump].text, strTostr(tog2).c_str());
												}
												nump++;
											}
										}
									}
								}

								if (lf->lineWidth_ > 0 && nl->getCount() > 1 && linn < nLINS && lo->getName().compare("page.userlayer.name") == 0)
									for (int k = 0; k < nl->getCount() - 1; k++)
									{
										linn++;
										alllins[linn].num = 1000 + ob->getBlockId();
										alllins[linn].color = 0;
										alllins[linn].type = lf->lineStyle_;
										SimplePoint* sp1 = nl->get(k)->getPos();
										SimplePoint* sp2 = nl->get(k + 1)->getPos();
										alllins[linn].x1 = (float)sp1->X();
										alllins[linn].y1 = (float)sp1->Y();
										alllins[linn].x2 = (float)sp2->X();
										alllins[linn].y2 = (float)sp2->Y();

										/*for (auto it2 = cn.begin(); it2 != cn.end(); it2++)
										{
											int bid = strToInt(it2->fields.at("block_id"), 10);
											int pid = strToInt(it2->fields.at("port_id"), 10);
											int cbid = strToInt(it2->fields.at("connector_id"), 10);
											int cpid = strToInt(it2->fields.at("connector_port_id"), 10);

											if (alllins[linn].num == bid)
											{
												alllins[linn].nobj[0] = bid;
												alllins[linn].tobj[0] = pid;
											}
											else if (alllins[linn].num == cbid)
											{
												alllins[linn].nobj[1] = cbid;
												alllins[linn].tobj[1] = cpid;
											}
										}*/
									}

								for (int iport = 0; iport < pl->getCount(); iport++)
								{
									if (inpn < nINPS)
									{
										PortObj* po = pl->get(iport);
										SimpleObj* so = (SimpleObj*)pl->get(iport);
										string pname = po->getName();
										SimplePoint* portpos = po->getPos();
										
										allinps[inpn].num = 1000 + so->getBlockId();
										allinps[inpn].color = 0;
										allinps[inpn].pos->x = (float)portpos->X();
										allinps[inpn].pos->y = (float)portpos->Y();
										if (po->getPortType() == 1)
										{
											allinps[inpn].pos->x += allobjs[i].pos.x;
											allinps[inpn].pos->y += allobjs[i].pos.y;
										}
										if (offset != nullptr)
										{ 
											allinps[inpn].pos->x -= offset->X();
											allinps[inpn].pos->y -= offset->Y();
										}
										allinps[inpn].t[0].vis = 1;
										strcpy(allinps[inpn].t[0].text, pname.c_str());
										allinps[inpn].parent = po->getParentId();

										for (auto it2 = ip.begin(); it2 != ip.end(); it2++)
										{
											if (idx == strToInt(it2->fields.at("block_id"), 10) &&
												//po->getParentId() == strToInt(it2->fields.at("fbport_parent_id"), 10) &&
												pname == it2->fields.at("fbport_name"))
											{
												allinps[inpn].negin = true;
											}
										}

										inpn++;
									}
								}

								
							}
						}
					}
				}
			}


			// *******************************  прорисовка всех объектов ubi-NT  ***************************

			int q1 = 0;
			LayerList* ll = page->getLayerList();
			for (int ki = 0; ki < ll->getCount(); ki++)
			{
				LayerObj* lo = ll->get(ki);
				String^ ln = gcnew String(lo->getName().c_str());
				if (lo != nullptr)
				for (int j = 0; j < lo->objectList_->objs_->size(); j++)
				{
					BasicObj* bo = lo->objectList_->get(j);
					string bn = bo->getName();
					unsigned int outtype = bo->getType();
					
					if (bn != "") // это составной объект
					{
						q1++;
						GroupBlockObj* fbo = (GroupBlockObj*)lo->objectList_->get(j);
						SimplePoint* offset = fbo->offset_;
						SimplePoint* sp = fbo->getPos();

						for (int q = 0; q < fbo->objectList_->objs_->size(); q++)
						{
							auto fbi = fbo->objectList_->get(q);

							if (fbi != nullptr)
							{
								unsigned int type = fbi->getType();
								unsigned int align = fbi->getTextAlingment();
								unsigned int bid = fbi->getBlockId();
								string en = fbi->getName();
								PortList* pl = fbi->getPortList();
								NodeList* nl = fbi->getNodeList();
								LineFormat* lf = fbi->getLineFormat();
								string tv = fbi->getTextValue();
								String^ tog2 = "";
								if (tv != "")
								{
									wchar_t* wstr = toUTF8(tv);
									tog2 = gcnew String(wstr);
									tog2 = tog2->Trim();
								}
								if (lf->lineWidth_ == 0 && tog2 != "" && en != "name" && en != "page_label")
								{
									SimplePoint* sp1 = fbi->getNodeList()->get(0)->getPos();
									SimplePoint* sp2 = fbi->getNodeList()->get(1)->getPos();

									float x1 = allobjs[q1].pos.x - offset->X();
									if (align == 0) x1 += (float)sp1->X();
									else if (align == 1) x1 += ((float)sp1->X() + (float)sp2->X()) / 2.0f - tog2->Length * 2.0f;
									else x1 += (float)sp2->X() - tog2->Length * 4.0f;
									float y1 = allobjs[q1].pos.y - offset->Y() +  (float)sp1->Y() + 1.0f; y1 = -y1;
									g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);
								}
								else if (type == 110 && lf->lineWidth_ > 0) // Rectangle
								{
									if (nl->getCount() == 4)
									{
										pen1->Width = lf->lineWidth_ * 1.0f * mas;
										SimplePoint* sp1 = nl->get(0)->getPos();
										SimplePoint* sp4 = nl->get(3)->getPos();
										float x1 = allobjs[q1].pos.x - offset->X() + (float)Math::Min(sp1->X(), sp4->X());
										float y1 = allobjs[q1].pos.y - offset->Y() + (float)Math::Min(sp1->Y(), sp4->Y()); y1 = -y1;
										float x4 = allobjs[q1].pos.x - offset->X() + (float)Math::Max(sp1->X(), sp4->X());
										float y4 = allobjs[q1].pos.y - offset->Y() + (float)Math::Max(sp1->Y(), sp4->Y()); y4 = -y4;
										g1->DrawRectangle(pen1, w / 2 + mas * x1, h / 2 - mas * (y1), mas * (x4 - x1), mas * (y1 - y4));
									}
								}
								else if (type == 115 && lf->lineWidth_ > 0) // Diamond
								{
									if (nl->getCount() == 4)
									{
										pen1->Width = lf->lineWidth_ * 1.0f * mas;
										SimplePoint* sp1 = nl->get(0)->getPos();
										SimplePoint* sp2 = nl->get(1)->getPos();
										SimplePoint* sp3 = nl->get(2)->getPos();
										SimplePoint* sp4 = nl->get(3)->getPos();
										float x1 = allobjs[q1].pos.x - offset->X() + ((float)sp1->X() + (float)sp2->X()) / 2.0f;
										float y1 = allobjs[q1].pos.y - offset->Y() + ((float)sp1->Y() + (float)sp2->Y()) / 2.0f; y1 = -y1;
										float x2 = allobjs[q1].pos.x - offset->X() + ((float)sp2->X() + (float)sp4->X()) / 2.0f;
										float y2 = allobjs[q1].pos.y - offset->Y() + ((float)sp2->Y() + (float)sp4->Y()) / 2.0f; y2 = -y2;
										float x3 = allobjs[q1].pos.x - offset->X() + ((float)sp3->X() + (float)sp4->X()) / 2.0f;
										float y3 = allobjs[q1].pos.y - offset->Y() + ((float)sp3->Y() + (float)sp4->Y()) / 2.0f; y3 = -y3;
										float x4 = allobjs[q1].pos.x - offset->X() + ((float)sp3->X() + (float)sp1->X()) / 2.0f;
										float y4 = allobjs[q1].pos.y - offset->Y() + ((float)sp3->Y() + (float)sp1->Y()) / 2.0f; y4 = -y4;
										g1->DrawLine(pen1, w / 2 + mas * x1, h / 2 - mas * y1, w / 2 + mas * x2, h / 2 - mas * y2);
										g1->DrawLine(pen1, w / 2 + mas * x2, h / 2 - mas * y2, w / 2 + mas * x3, h / 2 - mas * y3);
										g1->DrawLine(pen1, w / 2 + mas * x3, h / 2 - mas * y3, w / 2 + mas * x4, h / 2 - mas * y4);
										g1->DrawLine(pen1, w / 2 + mas * x4, h / 2 - mas * y4, w / 2 + mas * x1, h / 2 - mas * y1);
									}
								}
								else if (lf->lineWidth_ > 0)
								{
									ArrowObj* ao = fbi->getArrow();
									unsigned int start = 0;
									unsigned int end = 0;
									if (ao != nullptr)
									{
										start = ao->getStartArrow();
										end = ao->getEndArrow();
									}
										
									if (lf->lineStyle_ == 1)
									{
										//pen1->DashCap = System::Drawing::Drawing2D::DashCap::Round;
										pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
										cli::array<Single>^temp0 = { 1.5f * Math::Sqrt(Math::Sqrt(mas)), 1.8f * Math::Sqrt(Math::Sqrt(mas)) };
										pen1->DashPattern = temp0;
									}

									if (q >= fbo->objectList_->getCount()) //дополнительные линии
									{
										pen1->Color = Color::Brown;
										for (int k = 0; k < nl->getCount() - 1; k++)
										{
											pen1->Width = (unsigned int)lf->lineWidth_ * 0.5f * mas;
											SimplePoint* sp1 = nl->get(k)->getPos();
											SimplePoint* sp2 = nl->get(k + 1)->getPos();
											float x1 = (float)sp1->X();
											float y1 = (float)sp1->Y(); y1 = -y1;
											float x2 = (float)sp2->X();
											float y2 = (float)sp2->Y(); y2 = -y2;
											g1->DrawLine(pen1, w / 2 + mas * x1, h / 2 - mas * y1, w / 2 + mas * x2, h / 2 - mas * y2);
										}
										pen1->Color = Color::Black;
									}
									else if (nl->getCount() > 1)
									{
										for (int k = 0; k < nl->getCount() - 1; k++)
										{
											pen1->Width = lf->lineWidth_ * 1.0f * mas;
											SimplePoint* sp1 = nl->get(k)->getPos();
											SimplePoint* sp2 = nl->get(k + 1)->getPos();
											float x1 = allobjs[q1].pos.x - offset->X() + (float)sp1->X();
											float y1 = allobjs[q1].pos.y - offset->Y() + (float)sp1->Y(); y1 = -y1;
											float x2 = allobjs[q1].pos.x - offset->X() + (float)sp2->X();
											float y2 = allobjs[q1].pos.y - offset->Y() + (float)sp2->Y(); y2 = -y2;
											if (start == 6)
											{
												float dcx = -3.0f, dcy = -3.0f;
												if (fabs(x1 - x2) > 1) dcx -= 3.0f*(x1 - x2) / fabs(x1 - x2);
												if (fabs(y1 - y2) > 1) dcy += 3.0f*(y1 - y2) / fabs(y1 - y2);
												g1->DrawEllipse(pen2, w / 2 + mas * (x1 + dcx), h / 2 - mas * (y1 - dcy), mas*6.0f, mas*6.0f);
												if (fabs(x1 - x2) > 1) x1 -= 6.0f*(x1 - x2) / fabs(x1 - x2);
												if (fabs(y1 - y2) > 1) y1 -= 6.0f*(y1 - y2) / fabs(y1 - y2);
											}
											else if(start == 2)
											{
												float dcx = -3.0f, dcy = -3.0f;
												g1->DrawRectangle(pen2, w / 2 + mas * (x1 + dcx), h / 2 - mas * (y1 - dcy), mas*6.0f, mas*6.0f);
											}
											g1->DrawLine(pen1, w / 2 + mas * x1, h / 2 - mas * y1, w / 2 + mas * x2, h / 2 - mas * y2);
										}
									}
									pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
								}
							}

							for (int q2 = 0; q2 < 8; q2++) // прорисовка всех видимых свойств ubi
							if (allobjs[q1].t[q2].text[0] != '\0')
							{
								float x1 = allobjs[q1].t[q2].x - offset->X();
								float y1 = allobjs[q1].t[q2].y - offset->Y(); y1 = -y1;
								/*if (strstr(allobjs[q1].type, "rahmen") != NULL)
								{
									String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
									if (allobjs[q1].t[q2].vis == 1)
										g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);

								}
								else if (strstr(allobjs[q1].type, "abfsys") != NULL || strstr(allobjs[q1].type, "zuw.sys") != NULL)
								{
									String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
									if (allobjs[q1].t[q2].vis == 1) g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);
								}
								else*/
								{
									String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
									if (allobjs[q1].t[q2].vis == 1)
									{
										g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);
									}
								}
							}
						}
					}
					else if (bn == "" && outtype == 300) //300 - TextObj
					{
						string tv = bo->getTextValue();
						if (tv != "")
						{
							unsigned int align = bo->getTextAlingment();
							String^ tog2 = "";

							wchar_t* wstr = toUTF8(tv);
							tog2 = gcnew String(wstr);
							tog2 = tog2->Trim();
							SimplePoint* sp1 = bo->getNodeList()->get(0)->getPos();
							SimplePoint* sp2 = bo->getNodeList()->get(1)->getPos();

							float x1 = 0.0f;
							if (align == 0) x1 += (float)sp1->X();
							else if (align == 1) x1 += ((float)sp1->X() + (float)sp2->X()) / 2.0f - tog2->Length * 2.0f;
							else x1 += (float)sp2->X() - tog2->Length * 4.0f;
							float y1 = (float)sp1->Y() + 1.0f; y1 = -y1;
							g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);
						}
					}
				}
			}

			for (unsigned long q1 = 0; q1 <= nCONS; q1++)   // прорисовка всех коннекторов
				if (allcons[q1].num != 0)
				{
					String^ conname_s = gcnew String((char*)allcons[q1].conname.text);
					String^ conname_mark = "";
					allcons[q1].color = 0;
					float x1 = allcons[q1].conname.x;
					float y1 = allcons[q1].conname.y; y1 = -y1;

					if (allcons[q1].color == 0 && this->timer2->Enabled)
					{
						//поиск маркера - партнера и назначение цвета
					}

					g1->DrawString(conname_s, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);
					if (conname_mark != "") g1->DrawString(conname_mark, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1);

					for (int k = 0; k < 6; k++)
						if (allcons[q1].cons[k].vis == 1 && allcons[q1].cons[k].x != 0 && allcons[q1].cons[k].y != 0 && allcons[q1].cons[k].text[0] != '\0')
						{
							x1 = allcons[q1].cons[k].x;
							y1 = allcons[q1].cons[k].y; y1 = -y1;
							String^ tog2 = gcnew String((char*)allcons[q1].cons[k].text);
							if (tog2->Length < 21)  g1->DrawString(tog2, font1, System::Drawing::Brushes::DarkOrange, w / 2 + mas * x1, h / 2 - mas * y1);
							else g1->DrawString(tog2, font1, System::Drawing::Brushes::BlueViolet, w / 2 + mas * x1, h / 2 - mas * y1);
						}
				}

			for (int unsigned long q1 = 0; q1 <=nTABS; q1++)		// заполнение всех таблиц
				if (alltabs[q1].num != 0)
				{
					for (int k = 0; k < 10; k++)
						if (alltabs[q1].prop[k].vis == 1 && alltabs[q1].prop[k].text[0] != '\0' && alltabs[q1].prop[k].x != 0 && alltabs[q1].prop[k].y != 0)
						{
							String^ tog22 = gcnew String((char*)alltabs[q1].prop[k].text);
							tog22 = tog22->Trim();
							float x1 = alltabs[q1].prop[k].x;
							float y1 = alltabs[q1].prop[k].y; y1 = -y1;
							if (k == 7 && tog22->Length<21) g1->DrawString(tog22, font1, System::Drawing::Brushes::DarkOrange, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
							else if (k == 7 && tog22->Length >= 21) g1->DrawString(tog22, font1, System::Drawing::Brushes::BlueViolet, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
							else if (false && this->timer2->Enabled && k == 0 && tog22 != "")
							{
								/*String^ m_value = "";
								m_value = GetMarkerValue(awe_fstr, bgnum_fstr + "," + tog22->Replace(" ", "")->Replace(",0", ",")->Replace(",0", ",")->Replace("M0", "M")->Replace("M0", "M"), &alltabs[q1].color);

								if (m_value != "")
								{
									tog22 += "=" + m_value;
									deep = 0; ColorPartners(9, q1);
								}*/

								switch (alltabs[q1].color)
								{
								case 0:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
									break;
								case 1:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Blue, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
									break;
								case 2:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::LimeGreen, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
									break;
								case 3:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Magenta, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
									break;
								default:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
									break;
								}
							}
							else
							{
								g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w / 2 + mas * x1, h / 2 - mas * y1 - font1->SizeInPoints - mas + 1);
							}
						}
				}

			for (unsigned long q1 = 0; q1 <= nLINS; q1++)			// прорисовка всех линий
				if (alllins[q1].num != 0)
				{
					bool good = true;
					float point[4] = { 0, 0, 0, 0 };

					bool neg0 = false, neg1 = false;

					for (int q2 = 0; q2 <= nINPS; q2++)
					{
						if (fabs(allinps[q2].pos->x - alllins[q1].x1) < 1.0f && fabs(allinps[q2].pos->y - alllins[q1].y1) < 1.0f)
						{
							alllins[q1].nobj[0] = q2;
							alllins[q1].tobj[0] = 9;
							if (allinps[q2].negin)
							{
								neg0 = true;
								allinps[q2].negout = true;
							}
						}
						if (fabs(allinps[q2].pos->x - alllins[q1].x2) < 1.0f && fabs(allinps[q2].pos->y - alllins[q1].y2) < 1.0f)
						{
							alllins[q1].nobj[1] = q2;
							alllins[q1].tobj[1] = 9;
							if (allinps[q2].negin)
							{
								neg1 = true;
								allinps[q2].negout = true;
							}
						}
					}

					long n0 = alllins[q1].nobj[0];
					long t0 = alllins[q1].tobj[0];
					long n1 = alllins[q1].nobj[1];
					long t1 = alllins[q1].tobj[1];

					pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
					pen1->Width = 1.0f * mas;
					pen1->Color = Color::Black;

					switch (alllins[q1].color)
					{
					case 0:
						pen1->Color = Color::Black;
						break;
					case 1:
						pen1->Color = Color::Blue;
						break;
					case 2:
						pen1->Color = Color::LimeGreen;
						break;
					case 3:
						pen1->Color = Color::Magenta;
						break;
					default:
						pen1->Color = Color::Black;
						break;
					}
					if (alllins[q1].type == 1)
					{
						pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						cli::array<Single>^temp0 = { 1.5f, 3.6f };
						pen1->DashPattern = temp0;
					}

					if (t0 == 9 && n0 < 56) // линия, выходящая из таблицы
					{
						point[0] = alltabs[n0].pos.x;
						point[1] = alltabs[n0].pos.y;
						pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						alllins[q1].type = 1;
						cli::array<Single>^temp0 = { 1.5f, 3.6f };
						pen1->DashPattern = temp0;
					}
					else if (t0 == 9 && allinps[n0].num > 0) // линия, выходящая из элемента
					{
						if (!allinps[n0].link) allinps[n0].link = true;
						point[0] = allinps[n0].pos[0].x;
						point[1] = allinps[n0].pos[0].y;
					}
					else if (t0 == 2 && allpuns[n0].num > 0) // линия, выходящая из точки
					{
						point[0] = allpuns[n0].pos.x;
						point[1] = allpuns[n0].pos.y;
					}
					else good = false;

					if (t1 == 9 && n1 < 56) // линия, уходящая в таблицу
					{
						point[2] = alltabs[n1].pos.x;
						point[3] = alltabs[n1].pos.y;
						pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						alllins[q1].type = 1;
						cli::array<Single>^temp0 = { 1.5f, 3.6f};
						pen1->DashPattern = temp0;
					}
					else if (t1 == 9 && allinps[n1].num > 0) // линия, уходящая в элемент
					{
						if (!allinps[n1].link) allinps[n1].link = true;
						point[2] = allinps[n1].pos[0].x;
						point[3] = allinps[n1].pos[0].y;
					}
					else if (t1 == 2 && allpuns[n1].num > 0) // линия, уходящая в точку
					{
						point[2] = allpuns[n1].pos.x;
						point[3] = allpuns[n1].pos.y;
					}
					else good = false;

					if (true || good == 1)
					{
						point[0] = alllins[q1].x1;
						point[1] = - alllins[q1].y1;
						point[2] = alllins[q1].x2;
						point[3] = - alllins[q1].y2;
						float dcx = -3.0f, dcy = -3.0f;
						float inplen = 10.0f;
						if (neg0)
						{
							if (fabs(point[0] - point[2]) > 1) dcx -= 3.0f*(point[0] - point[2]) / fabs(point[0] - point[2]);
							if (fabs(point[1] - point[3]) > 1) dcy += 3.0f*(point[1] - point[3]) / fabs(point[1] - point[3]);
							g1->DrawEllipse(pen2, w / 2 + mas * (point[0] + dcx), h / 2 - mas * (point[1] - dcy), mas*6.0f, mas*6.0f);
							if (fabs(point[0] - point[2]) > 1) point[0] -= 6.0f*(point[0] - point[2]) / fabs(point[0] - point[2]);
							if (fabs(point[1] - point[3]) > 1) point[1] -= 6.0f*(point[1] - point[3]) / fabs(point[1] - point[3]);
							inplen = 3.2f;
						}
						else if (neg1)
						{
							if (fabs(point[0] - point[2]) > 1) dcx += 3.0f*(point[0] - point[2]) / fabs(point[0] - point[2]);
							if (fabs(point[1] - point[3]) > 1) dcy -= 3.0f*(point[1] - point[3]) / fabs(point[1] - point[3]);
							g1->DrawEllipse(pen2, w / 2 + mas * (point[2] + dcx), h / 2 - mas * (point[3] - dcy), mas*6.0f, mas*6.0f);
							if (fabs(point[0] - point[2]) > 1) point[2] += 6.0f*(point[0] - point[2]) / fabs(point[0] - point[2]);
							if (fabs(point[1] - point[3]) > 1) point[3] += 6.0f*(point[1] - point[3]) / fabs(point[1] - point[3]);
							inplen = 3.2f;
						}
						g1->DrawLine(pen1, w / 2 + mas * point[0], h / 2 - mas * point[1], w / 2 + mas * point[2], h / 2 - mas * point[3]);
						g1->DrawLine(pen1, w / 2 + mas * point[2], h / 2 - mas * point[3], w / 2 + mas * (point[2] - (point[2] - point[0]) / fabs(point[2] - point[0])*inplen), h / 2 - mas * point[3]);
						g1->DrawLine(pen1, w / 2 + mas * point[0], h / 2 - mas * point[1], w / 2 + mas * (point[0] - (point[0] - point[2]) / fabs(point[0] - point[2])*inplen), h / 2 - mas * point[1]);

						if (t0 == 9 && n0 < 56 && t1 == 9 && n1 >= 56)
						{
							pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
							g1->DrawLine(pen1, w / 2 + mas * point[2], h / 2 - mas * point[3], w / 2 + mas * (point[2] - (point[2] - point[0]) / fabs(point[2] - point[0])*inplen), h / 2 - mas * point[3]);
						}
						else if (t0 == 9 && n0 >= 56 && t1 == 9 && n1 < 56)
						{
							pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
							g1->DrawLine(pen1, w / 2 + mas * point[0], h / 2 - mas * point[1], w / 2 + mas * (point[0] - (point[0] - point[2]) / fabs(point[0] - point[2])*inplen), h / 2 - mas * point[1]);
						}
					}
				}


				for (unsigned long q1 = 0; q1 <= nINPS; q1++)	// прорисовка всех входов/выходов
					if (allinps[q1].num != 0)
					{
						// прорисовка линии входа/выхода
						switch (allinps[q1].color)
						{
						case 0:
							pen1->Color = Color::Black;
							break;
						case 1:
							pen1->Color = Color::Blue;
							break;
						case 2:
							pen1->Color = Color::LimeGreen;
							break;
						case 3:
							pen1->Color = Color::Magenta;
							break;
						default:
							pen1->Color = Color::Black;
							break;
						}

						if (allinps[q1].negout)
						{
							float radius = sqrt((allinps[q1].circlepos[0] - allinps[q1].circlepos[2])*(allinps[q1].circlepos[0] - allinps[q1].circlepos[2]) + (allinps[q1].circlepos[1] - allinps[q1].circlepos[3])*(allinps[q1].circlepos[1] - allinps[q1].circlepos[3]));
							g1->DrawEllipse(pen2, w / 2 + mas * (allinps[q1].circlepos[0] - radius), h / 2 - mas * (allinps[q1].circlepos[1] + radius), mas*2.0f*radius, mas*2.0f*radius);
						}

						String^ tog2 = "";
						//pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;

						unsigned char nvis = 0;
						for (int k = 0; k < 8; k++) if (allinps[q1].t[k].vis == 1) nvis++;

						for (int k = 0; k < 8; k++)
							if (allinps[q1].t[k].vis == 1) // рисовать только видимые надписи
							{
								if (nvis <= 2 /*&& allinps[q1].name[0] == 'Z' || allinps[q1].t[k].text[0] == 'E'*/)
								{
									String^ tog2 = gcnew String((char*)allinps[q1].t[k].text);

									if (allinps[q1].negin && !allinps[q1].negout)
									{
										float radius = 3.0f;
										float x1 = (allinps[q1].pos->x - radius);
										float y1 = (allinps[q1].pos->y - radius); y1 = -y1;
										g1->DrawEllipse(pen1, w / 2 + mas * x1, h / 2 - mas * y1, mas * 2.0f * radius, mas * 2.0f * radius);
									}

									if (/*this->timer2->Enabled &&*/ tog2 != "") // определение цвета по значению маркера
									{
										float radius = 1.5f;
										float x1 = (allinps[q1].pos->x - radius);
										float y1 = (allinps[q1].pos->y - radius); y1 = -y1;
										g1->FillEllipse(System::Drawing::Brushes::SteelBlue, w / 2 + mas * x1, h / 2 - mas * y1, mas * 2.0f * radius, mas * 2.0f * radius);
									}
								}
								else
								{
									allinps[q1].t[k].vis = 2;
								}
							}
						if (allinps[q1].num != 0 && this->debugInfoToolStripMenuItem->Checked) g1->DrawString(Convert::ToString((unsigned char)allinps[q1].num), font1, System::Drawing::Brushes::Green, w / 2 + mas * allinps[q1].pos[0].x, h / 2 - mas * (allinps[q1].pos[0].y + 1) - font1->SizeInPoints - mas + 1);


						

						

					}

			fg1->DrawImage(canvBitmap, PointF(0, 0));

			oldindex = newindex;
		}
		catch (Exception^ e)
		{
			DWORD err = GetLastError();
			String^ src = "";
			if (e->StackTrace->Contains("строка")) src = e->StackTrace->Substring(e->StackTrace->IndexOf("строка"));
			listBox2->Items->Add(e->StackTrace);
			listBox2->Items->Add(e->Message);
			listBox2->Items->Add(Convert::ToString((int)err));
			listBox2->TopIndex = listBox2->Items->Count - 1;

			this->toolStripLabel1->Text += src + e->Message + Convert::ToString((int)err);
			
		};
	}
	else if (tabControl1->SelectedIndex == 0)
	{
	try{
			this->listBox1->SelectedIndex = -1;
			 if (this->SearchResults->SelectedIndex >= 0 && this->SearchResults->SelectedItem->ToString()->Length > 0) 
			 {
				newindex=this->slist.IndexOfValue(this->SearchResults->SelectedItem->ToString());
				w = this->pictureBox1->Width  +  dx*2;
				h = this->pictureBox1->Height  +  dy*2;
				g1->Clear(*myforecolor);
				pen1->Width = mas*0.35f;
				pen2->Width = mas*0.35f;
				if (newindex>=0 && this->slist.Keys[newindex]->Contains("\\yf\\yf#=") ) DrawElf(g1,"ubi.fo.fup", 0, 0);
				else if (newindex>=0 && this->slist.Keys[newindex]->Contains("\\ym\\ym#=") ) DrawElf(g1,"ubi.fo.mes", 0, 0);
			 }
			 else newindex=-1;

			//long addrindex = this->slist.IndexOfKey(this->SearchResults->SelectedItem->ToString());
		
			 if (newindex!=oldindex && newindex>=0)
			 {
				 if (!undoredo) 
				 {
					 if (histpos<1) this->UndoButton->DropDownItems->Add(this->SearchResults->SelectedItem->ToString());
					 else
					 {
						 this->UndoButton->DropDownItems->Add(this->SearchResults->SelectedItem->ToString());
						 for (int q1=this->UndoButton->DropDownItems->Count-1; q1>histpos+1; q1--)
						 {
							this->UndoButton->DropDownItems[q1]->Text = this->UndoButton->DropDownItems[q1-1]->Text;
						 }
						 this->UndoButton->DropDownItems[histpos+1]->Text = this->SearchResults->SelectedItem->ToString();
					 }
					 if (histpos>=0) this->UndoButton->DropDownItems[histpos]->BackColor = SystemColors::Control;
					 histpos++;
					 if (histpos>25) 
					 {
						 this->UndoButton->DropDownItems->RemoveAt(0);
						 histpos--;
					 }
				 }
				 else undoredo=false;
				 if (histpos>=0) this->UndoButton->DropDownItems[histpos]->BackColor = Color::DarkGray;

				 if (allpuns!=NULL) { std::memset(&allpuns,0,sizeof(allpuns)); delete allpuns; nPUNS = 0; }
				 if (punsadd!=NULL) { std::memset(&punsadd,0,sizeof(punsadd)); delete punsadd;}
				 if (alllins!=NULL) { std::memset(&alllins,0,sizeof(alllins)); delete alllins; nLINS = 0; }
				 if (linsadd!=NULL) { std::memset(&linsadd,0,sizeof(linsadd)); delete linsadd;}
				 if (allcons!=NULL) { std::memset(&allcons,0,sizeof(allcons)); delete allcons; nCONS = 0; }
				 if (alltabs!=NULL) { std::memset(&alltabs,0,sizeof(alltabs)); delete alltabs; nTABS = 0; }
				 if (allobjs!=NULL) { std::memset(&allobjs,0,sizeof(allobjs)); delete allobjs; nOBJS = 0; }
				 if (allacts!=NULL) { std::memset(&allacts,0,sizeof(allacts)); delete allacts; nINPS = 0; }

				 std::memset(&info,0,sizeof(info));

				 char *curkks, *curlist;
				 int kksend = slist.Values[newindex]->IndexOf('#');
				 if (kksend>0 && kksend<slist.Values[newindex]->Length)
				 {
					 curkks = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(slist.Values[newindex]->ToUpper()->Substring(0,kksend)).ToPointer();
					 curlist = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(slist.Values[newindex]->ToUpper()->Substring(kksend+1)).ToPointer();
					 memcpy(&info.kks[0],&curkks[0],kksend);
					 memcpy(&info.list[0],&curlist[0],slist.Values[newindex]->Length-kksend-1);
				 }


				 String^ infopath_s = this->slist.Keys[newindex]->Replace("\\fup.gra","\\fup.sfd");
				 if (infopath_s->Contains("#1.") && !infopath_s->Contains(".akt\\")) infopath_s = infopath_s->Substring(0,infopath_s->IndexOf("#1.")+2);
				 char *infopath = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(infopath_s).ToPointer();
				 FILE* infoin;
				 fopen_s(&infoin,infopath,"rb");

				 if (infoin!=NULL)
				 {
					 unsigned char t1;
					 unsigned char parname[256];
					 unsigned char parvalue[256];
					 int namelen=0;
					 int valuelen=-1;
					 do
					 {
						 std::fread(&t1,1,1,infoin);
						if (t1!='|' && t1!=0x0A && t1!=0x0D && namelen>=0)
						{
							parname[namelen] = t1;
							namelen++;
						}
						else if (t1=='|' && namelen>=0)
						{
							parname[namelen] = '\0';
							namelen=-1;
							valuelen=0;
						}
						else if (t1!='|' && valuelen>=0)
						{
							parvalue[valuelen] = t1;
							valuelen++;
						}
						else if (t1=='|' && valuelen>=0)
						{
							parvalue[valuelen] = '\0';
							if (valuelen>1)
							{
								String^ parname_s = gcnew String((char*)parname);
								String^ parvalue_s = gcnew String((char*)parvalue);
								if (parname_s == "uaso") memcpy(&info.type[0],&parvalue[0],valuelen+1);
								else if (parname_s == "ada1") memcpy(&info.date[0][0],&parvalue[0],valuelen+1);
								else if (parname_s == "ada2") memcpy(&info.date[1][0],&parvalue[0],valuelen+1);
								else if (parname_s == "ada3") memcpy(&info.date[2][0],&parvalue[0],valuelen+1);
								else if (parname_s == "ben2") 
								{
									memcpy(&info.name[0],&parvalue[0],valuelen+1);
								}
								else if (parname_s == "ort") memcpy(&info.awename[0],&parvalue[0],valuelen+1);
								//else if (parname_s == "unr1") memcpy(&info.bpotype[0],&parvalue[namelen],valuelen);
							}
							valuelen=-1;
							namelen=0;
						}
					 } while (!feof(infoin));
				 
				 }

				 String^ infopath2_s = this->slist.Keys[newindex]->Replace(".akt\\","\\")->Replace("\\fup.gra\\","\\fup.inf\\");
				 if (infopath2_s->Contains("#1.")) infopath2_s = infopath2_s->Substring(0,infopath2_s->IndexOf("#1.")+2);
				 char *infopath2 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(infopath2_s).ToPointer();
				 FILE* infoin2;
				 fopen_s(&infoin2,infopath2,"r");
				 if (infoin2!=NULL)
				 {
					 fscanf(infoin2,"%s %s %s", &info.awenum[0], &info.bgtype[0], &info.bgnum[0]);
					 info.awenum[10]='\0';
					 info.bgtype[3]='\0';
					 info.bgnum[3]='\0';
					 std::fclose(infoin2);
				 }

				 this->listBox2->Items->Clear();
				 char *bigpath = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->slist.Keys[newindex]).ToPointer();
				 FILE* bigin;
				 fopen_s(&bigin,bigpath,"rb");

				 unsigned char t1, t2;
				 long n=0;
				 nacts=0;
				 unsigned long tubi=0x00000000;

				 fpos_t fpos1=0;
				 fpos_t fposb=0;
				 bool fposbable = true;
				 bool oldversion = false;

				 unsigned long nPUNSc=0;
				 unsigned long nLINSc=0;

				 fpos_t segbegpos = 0;
				 fpos_t segendpos = 0;
				 fpos_t segcurpos = 0x150*16;

				 if (bigin!=NULL) std::fsetpos(bigin,&segcurpos);

				 if (bigin!=NULL) do
				 {
					 n++;
					 std::fread(&t1,1,1,bigin);
					 tubi = (tubi << 8) + t1;

					 if ( tubi == 0x70756E6B && nPUNS == 0)  // Сколько всего пунктов?
					 {
						 for (int i=0; i<5; i++) { std::fread(&t1,1,1,bigin); tubi = (tubi << 8) + t1;}
						 for (int i=0; i<4; i++) if (t1==0x00) std::fread(&t1,1,1,bigin);

						 if (t1!=0x00 && ((tubi&0xFFFFFF00) == 0x20202000)) 
						 {
							 nPUNS=t1;
							 std::fread(&t1,1,1,bigin);
							 if (t1!=0x00) nPUNS = (nPUNS<<8) + t1;
							 if (nPUNS<0 || nPUNS>300) nPUNS=300;
							 allpuns = new PUNS[nPUNS+1];
							 punsadd = new unsigned long [nPUNS+1];
							 std::memset(allpuns,0,sizeof(PUNS)*(nPUNS+1));
							 std::memset(punsadd,0,4*(nPUNS+1));
							 this->listBox2->Items->Add("nPUNS="+Convert::ToString((long)nPUNS));
						 }
					 }
					 else if ( tubi == 0x76702020 )  // Адрес найденного пункта
					 {
						unsigned char buf[4];
						if (nPUNSc==0) nPUNSc=1;
						else if (nPUNSc <= nPUNS)
						{
							std::fgetpos(bigin,&segcurpos); segcurpos-=18; std::fsetpos(bigin,&segcurpos);
							std::fread(&buf[0],1,4,bigin);
							punsadd[nPUNSc] = buf[0] + (buf[1]<<8) + (buf[2]<<16) + (buf[3]<<24);
							if ((buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3] < punsadd[nPUNSc]) 
							{
								oldversion = true;
								punsadd[nPUNSc] = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3]; 
							}
							this->listBox2->Items->Add("punkt["+ Convert::ToString((long)nPUNSc)+"] add = "+Convert::ToString((long)punsadd[nPUNSc]));
							nPUNSc++;
							segcurpos+=20; std::fsetpos(bigin,&segcurpos);
						}
					 }					 
					 else if ( tubi == 0x6C696E69 && nLINS == 0)  // Сколько всего линий?
					 {
						 long tmp=0;
						 for (int i=0; i<5; i++) { std::fread(&t1,1,1,bigin); tubi = (tubi << 8) + t1;}
						 for (int i=0; i<4; i++) if (t1==0x00) std::fread(&t1,1,1,bigin);

						 if (t1!=0x00 && ((tubi&0xFFFFFF00) == 0x20202000)) 
						 {
							 nLINS=t1; 
							 std::fread(&t1,1,1,bigin);
							 if (t1!=0x00) nLINS = (nLINS<<8) + t1;
							 if (nLINS<0 || nLINS>300) nLINS=300;
							 alllins = new LINS[nLINS+1];
							 linsadd = new unsigned long [nLINS+1];
							 std::memset(alllins,0,sizeof(LINS)*(nLINS+1));
							 std::memset(linsadd,0,4*(nLINS+1));
							 this->listBox2->Items->Add("nLINS="+Convert::ToString((long)nLINS));
						 }
					 }
					 else if ( tubi == 0x766C2020 )  // Адрес найденной линии
					 {
						 unsigned char buf[4];
						 if (nLINSc==0) nLINSc = 1;
						 else if (nLINSc <= nLINS)
						 {
							 std::fgetpos(bigin,&segcurpos); segcurpos-=18; std::fsetpos(bigin,&segcurpos);
							 std::fread(&buf[0],1,4,bigin);
							linsadd[nLINSc] = buf[0] + (buf[1]<<8) + (buf[2]<<16) + (buf[3]<<24);
							if ((buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3] < linsadd[nLINSc]) 
							{
								oldversion = true;
								linsadd[nLINSc] = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3]; 
							}
							this->listBox2->Items->Add("line["+ Convert::ToString((long)nLINSc)+"] add = "+Convert::ToString((long)linsadd[nLINSc]));
							nLINSc++;
							segcurpos+=20; std::fsetpos(bigin,&segcurpos);
						 }
					 }
					 else if ( tubi == 0x00000978 )  // описание выходных и выходных таблиц
					 {
						 //fposbable=true; //где-то за ними пойдет первородный элемент
					 }
			 
					 else if ( tubi == 0x62737420 && nOBJS == 0) // Сколько всего объектов?
					 {
						 for (int i=0; i<5; i++) { std::fread(&t1,1,1,bigin); tubi = (tubi << 8) + t1;}
						 for (int i=0; i<4; i++)
							 if (t1==0x00 && ((tubi & 0xFFFFFF00) == 0x20202000) )
								 std::fread(&t1,1,1,bigin);

						 if (t1!=0x00 && ((tubi & 0xFFFFFF00) == 0x20202000)) 
						 {nOBJS=t1;
						 if (nOBJS<0 || nOBJS>100) nOBJS=100;
						 allobjs = new OBJS[nOBJS+1];
						 std::memset(allobjs,0,sizeof(OBJS)*(nOBJS+1));
						 this->listBox2->Items->Add("nOBJS="+Convert::ToString((long)nOBJS));}
					 }
					 else if ( tubi == 0x62612020 && nINPS == 0) // Сколько всего входов/выходов?
					 {
						 for (int i=0; i<5; i++) { std::fread(&t1,1,1,bigin); tubi = (tubi << 8) + t1;}
						 for (int i=0; i<4; i++) if (t1==0x00) std::fread(&t1,1,1,bigin);

						 if (t1!=0x00 && ((tubi&0xFFFFFF00) == 0x20202000))
						 {
							 nINPS = t1;
							 nTABS = t1;
							 std::fread(&t1,1,1,bigin);
							 if (t1!=0x00) nINPS = (nINPS<<8) + t1;
							 if (nINPS<0 || nINPS>500) nINPS=500;
							 allinps = new INPS[nINPS+1];
							 std::memset(allinps,0,sizeof(INPS)*(nINPS+1));
							 alltabs = new TABS[nTABS +1];
							 std::memset(alltabs,0,sizeof(TABS)*(nTABS +1));
							 this->listBox2->Items->Add("nINPS="+Convert::ToString((long)nINPS));
						 }
					 }
					 else if ( tubi == 0x61732020 && nCONS == 0) // Сколько всего коннекторов?
					 {
						 for (int i=0; i<5; i++) { std::fread(&t1,1,1,bigin); tubi = (tubi << 8) + t1;}
						 for (int i=0; i<4; i++) if (t1==0x00) std::fread(&t1,1,1,bigin);

						 if (t1!=0x00 && ((tubi&0xFFFFFF00) == 0x20202000)) 
						 {
							 nCONS=t1;
							 std::fread(&t1,1,1,bigin);
							 if (t1!=0x00) nCONS = (nCONS<<8) + t1;
							 if (nCONS<0 || nCONS>100) nCONS=100;
							 allcons = new CONS[nCONS+1];
							 std::memset(allcons,0,sizeof(CONS)*(nCONS+1));
							 allacts = new ACTS[nINPS+nCONS];
							 std::memset(allacts,0,sizeof(ACTS)*(nINPS+nCONS));
							 this->listBox2->Items->Add("nCONS="+Convert::ToString((long)nCONS));
						 }
					 }
					 else if ( fposbable && (  ((tubi & 0x0000FFFF) == 0x00000106 && !oldversion) || ((tubi & 0x0000FFFF) == 0x00000601) && oldversion) ) // firstseg
					 {
						 tubi = 0x00000000;
						 for (int i=0; i<4; i++) 
						 {
							 std::fread(&t1,1,1,bigin);
							 if (oldversion) tubi = tubi | (t1 << (i*8));
							 else tubi = (tubi << 8) + t1;
						 }
						 if ( (tubi&0x0000FFFF) <= 0x0A && fposbable && fposb==0) // Первородный элемент
						 {
							 std::fgetpos(bigin,&segendpos);
							 fposb=segendpos-8; 
							 fposbable=false;
							 this->listBox2->Items->Add("fposb="+Convert::ToString((long)fposb)); 
							 // если к этому моменту нулевое количество каких-либо элементов...
							 if (nCONS==0) 
							 {	
								allacts = new ACTS[nINPS+nCONS];
								std::memset(allacts,0,sizeof(ACTS)*(nINPS+nCONS));
							 }
							 std::fsetpos(bigin,&fposb);
							 if (oldversion) GetAlf2Rus((unsigned char*)&info.name[0]);
							 else GetAlf2RusOld((unsigned char*)&info.name[0]);
						 }
					 }
					 else if ( !fposbable && (  ((tubi & 0x0000FFFF) == 0x00000106 && !oldversion) || ((tubi & 0x0000FFFF) == 0x00000601) && oldversion) ) // newseg
					 {
						 unsigned char buf[1024];
						 long seglen = ((tubi & 0x00FF0000) >> 8) + ((tubi & 0xFF000000) >> 24);
						 if (oldversion) 
						 {
							 seglen = (tubi & 0xFFFF0000) >> 16;
						 }
					 
						 std::fgetpos(bigin,&segendpos);
						 segendpos = segendpos-4 + seglen*2;

						 tubi = 0x00000000;
						 for (int i=0; i<4; i++) 
						 {
							 std::fread(&t1,1,1,bigin);
							 if (oldversion) tubi = tubi | (t1 << (i*8));
							 else tubi = (tubi << 8) + t1;
						 }
						 unsigned long segnum = ((tubi & 0xFF000000) >> 24) + ((tubi & 0x00FF0000) >> 8);

						 unsigned long templong=0x00000000;
						 for (int i=0; i<4; i++)	// поиск названия сегмента
						 {			
							 std::fread(&t1,1,1,bigin);
							templong = (templong << 8) + t1;
						 } //while (((templong & 0xFF00FFFF) != 0x0200000C ) && seglen>0);
						 if (oldversion) templong = (templong & 0x0000FFFF) | ((templong & 0xFF000000) >> 8) | ((templong & 0x00FF0000) << 8);

						 char segname[25];
						 segname[0]='\0';
						 long segnamelen = 0;
						 //if 

						 if ((templong & 0xFF00FFFF) == 0x0200000C)
						 {
							 segnamelen = ((templong & 0x00FF0000)>>16) - 6;
							 std::fread((void*)buf,1,2,bigin);
							 if (segnamelen<21) 
							 {
								 std::fread((void*)&segname[0],1,segnamelen,bigin);
								segname[segnamelen]='\0';
							 }
							 if (segnamelen%2==1) { std::fread(&t1,1,1,bigin);}
						 }
						 else //  нет названия, например у линий, пунктов и прочего
						 {
							 std::fgetpos(bigin,&fpos1);
							 fpos1-=4;
							 std::fsetpos(bigin,&fpos1);
						 }

						 if ( (segname[0]=='l' || segname[0]=='r') && segname[1]=='_' && seglen == 0xEB && !oldversion) // empty table
						 {
							 alltabs[segnum].num = 0;
							 std::fsetpos(bigin,&segendpos);
							 //fread((void*)buf,1,452,bigin);
							 tubi = 0x00000000;
						 }
						 else if ( (segname[0]=='l' || segname[0]=='r') && segname[1]=='_' && (seglen == 0xD6 || seglen == 0x70)) // empty table
						 {
							 alltabs[segnum].num = 0;
							 std::fsetpos(bigin,&segendpos);
							 //fread((void*)buf,1,400,bigin);
							 tubi = 0x00000000;
						 } 
						 else if (( (segname[0]=='l' || segname[0]=='r') && segname[1]=='_' && ( (seglen != 0xEB) || oldversion) && seglen != 0xD6 && seglen != 0x70 && segnum <= nINPS)
								|| (segname[0]=='l' && segname[1]=='_' && segname[2]=='0' && segname[3]=='0' && segnum <= nINPS)) // table
						 {
						 
							 std::fread((void*)buf,1,26,bigin);
							 float tablinepos[2] = {0,0};
							 for (int j=0; j<2; j++)
							 {
								 unsigned char part[4];
								 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
								 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
								 tablinepos[j] = DWord2Float(part);
							 }
							 alltabs[segnum].num = segnum;
							 alltabs[segnum].color = 0;
							 alltabs[segnum].row[0] = segname[2]; alltabs[segnum].row[1] = segname[3];
							 if (alltabs[segnum].row[0]=='0') alltabs[segnum].row[0]=' ';
							 alltabs[segnum].pos.x  = tablinepos[0]; 
							 alltabs[segnum].pos.y  = tablinepos[1];

							 this->listBox2->Items->Add("Tab"+Convert::ToString((long)segnum)+"=["+Convert::ToString((char*)segname)+"] Pos=["+Convert::ToString(tablinepos[0])+","+Convert::ToString(tablinepos[1])+"]");

							 /*pen1->Brush = Brushes::Red;
							 g1->DrawLine(pen1, w/2+mas*tablinepos[0]+3, h/2-mas*tablinepos[1]+3, w/2+mas*tablinepos[0]-3, h/2-mas*tablinepos[1]-3);
							 g1->DrawLine(pen1, w/2+mas*tablinepos[0]-3, h/2-mas*tablinepos[1]+3, w/2+mas*tablinepos[0]+3, h/2-mas*tablinepos[1]-3);
							 g1->DrawString(Convert::ToString((unsigned char)segnum),  font1, System::Drawing::Brushes::Red, w/2+mas*tablinepos[0], h/2-mas*(tablinepos[1]+1)-font1->SizeInPoints -mas+1);
							 font1->SizeInPoints = 2.0f*mas;
							 pen1->Brush = Brushes::Black;*/

							 unsigned char ntmax=10;
							 if (segname[0]=='l' && segname[1]=='_' && segname[2]=='0' && segname[3]=='0') ntmax=8;

							 for (int nt=0; nt<ntmax; nt++)
							 {
								 unsigned long textlen=0;
								 while ((tubi&0xFF00FFFF)!=0x0200060C && (tubi&0x00FFFFFF)!=0x0002060C && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-8) )
								 {
									 std::fread(&t1,1,1,bigin);
									 tubi = (tubi << 8) + t1;
								 }
								 if ( ((tubi&0xFF00FFFF)==0x0200060C) || ((tubi&0x00FFFFFF)==0x0002060C) )
								 {
									 long tt = ((tubi&0x00FF0000)>>16) - 4;
									 if (oldversion) tt = ((tubi&0xFF000000)>>24) - 4;
									 textlen = 0;
									 std::fread((void*)buf,1,4,bigin);
									 tt-=4;

									 do
									 {
										 if (t1!=0x00)
										 {
											 std::fread(&t1,1,1,bigin);
											tubi = (tubi << 8) + t1;
										 }
										 else
										 {
											 std::fread(&t1,1,1,bigin);
										 }
										 tt--;
									 } while ((tubi&0xFFFF00FF)!=0x9999005E && (tubi&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-8) );
									 textlen = tt-8;
					 
									 float textpos[2] = {0,0};
									 for (int j=0; j<2; j++)
									 {
										 unsigned char part[4];
										 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
										 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
										 textpos[j] = DWord2Float(part);
									 }
									 alltabs[segnum].prop[nt].x = textpos[0];
									 alltabs[segnum].prop[nt].y = textpos[1];
									 alltabs[segnum].prop[nt].text[0] = '\0';

									 if (textlen > 0) 
									 {
										 alltabs[segnum].prop[nt].vis = 1;
										 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
										 if ((nt>7 || ntmax==8) && !oldversion) GetAlf2Rus(buf);
										 else if ((nt>7 || ntmax==8) && oldversion) GetAlf2RusOld(buf);
										 memcpy(&alltabs[segnum].prop[nt].text,&buf[0],21); buf[textlen]='\0';

										 if (nt==2 && ntmax==10) 
										 {
											 nacts++;	// add tab to acts
											 allacts[nacts].num = nacts;
											 allacts[nacts].zone.x1 = textpos[0];
											 allacts[nacts].zone.y1 = textpos[1];
											 allacts[nacts].zone.x2 = textpos[0]+textlen*2.1f;
											 allacts[nacts].zone.y2 = textpos[1]+3.0f;
											 memcpy(&allacts[nacts].list[0],&buf[0],textlen+1);
											 memcpy(&allacts[nacts].kks[0],&alltabs[segnum].prop[1].text,15);
										 }					 	 
									 }
									 //if (tt%2==1) fread(&t1,1,1,bigin); 
									 tubi=0x00000000;
								 }
							 }
						
						 }
						 else if ( (tubi & 0x0000FFFF) == 0x00000009 && segnum <= nINPS)  // inps
						 {
							std::fgetpos(bigin,&fpos1);
							 long oldseglen=seglen;

							 memcpy(&allinps[segnum].name[0],&segname[0],segnamelen+1);

							 do {				// поиск координаты входа/выхода
								 std::fread(&t1,1,1,bigin);
								tubi = (tubi << 8) + t1;
							 } while (tubi!=0x04060500 && tubi!=0x04060005 && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) );

							 float inppos2[2] = {0,0};
							 for (int j=0; j<2; j++)
							 {
								 unsigned char part[4];
								 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
								 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
								 inppos2[j] = DWord2Float(part);
							 }
							 allinps[segnum].num = segnum;
							 allinps[segnum].color = 0;
							 allinps[segnum].pos[0].x  = inppos2[0];
							 allinps[segnum].pos[0].y  = inppos2[1];
							 pen1->Brush = Brushes::Green;
							 //font1->SizeInPoints = 1.0f*mas;
							 //g1->DrawString(Convert::ToString((unsigned char)segnum), font1, System::Drawing::Brushes::Green, w/2+mas*inppos2[0], h/2-mas*(inppos2[1]+1)-font1->SizeInPoints -mas+1);
							 pen1->Brush = Brushes::Black;

							 allinps[segnum].pos[1].x = allinps[segnum].pos[0].x;
							 allinps[segnum].pos[1].y = allinps[segnum].pos[0].y;

							 tubi = 0x00000000;

							 unsigned char nline = 1;
							 unsigned char ntext = 0;

							 // линии у входа/выхода, или тексты
							 while ( ((tubi & 0x0000FFFF) != 0x00006C69) && ((tubi & 0xFF00FFFF) != 0x0200060C) && ((tubi & 0x00FFFFFF) != 0x0002060C) && ((tubi & 0x0000FFFF) != 0x00006B72) && !std::fgetpos(bigin,&segcurpos) && segcurpos<segendpos-4  && ((tubi & 0x0000FFFF) != 0x00000106) && ((tubi & 0x0000FFFF) != 0x00000601))
							 {
								 std::fread(&t1,1,1,bigin);
								 tubi = (tubi << 8) + t1;

								 if ((tubi & 0x0000FFFF) == 0x00006C69)  // первая по счету линия у входа/выхода
								 {
									 std::fread((void*)buf,1,6,bigin);
									 float point[4] = {0,0,0,0};
									 for (int j=0; j<4; j++)
									 {
										 unsigned char part[4];
										 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
										 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
										 point[j] = DWord2Float(part);
									 }
									 allinps[segnum].pos[nline].x = point[2];
									 allinps[segnum].pos[nline].y = point[3];
									 tubi = 0x00000000;	
									 if (nline<=4) nline++;
								 
								 }
								 else if ( ((tubi & 0x00FFFFFF) == 0x0002060C) || ((tubi & 0xFF00FFFF) == 0x0200060C) )  // а это нашелся текст возле входа/выхода
									  //&& ((tubi & 0x00FF0000)>>16) > 0x22 ) ) 
								 {
									 unsigned char alignhor=0;
									 unsigned char visible=0;

									 long tt=((tubi & 0x00FF0000) >> 16) - 4;
									 if (oldversion) tt=((tubi & 0xFF000000) >> 24) - 4;
									 long textlen=0;

									 std::fread((void*)buf,1,2,bigin);
									 std::fread((void*)&visible,1,1,bigin);
									 std::fread((void*)&alignhor,1,1,bigin);
									 tt-=4;
								 
									 do
									 {
										 if (t1!=0x00)
										 {
											 std::fread(&t1,1,1,bigin);
											tubi = (tubi << 8) + t1;
										 }
										 else
										 {
											 std::fread(&t1,1,1,bigin);
										 }
										 tt--;
									 } while ((tubi&0xFFFF00FF)!=0x9999005E && (tubi&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<segendpos-4);
									 textlen = tt-8;

									 float textpos[2] = {0,0};
									 for (int j=0; j<2; j++)
									 {
										 unsigned char part[4];
										 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
										 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
										 textpos[j] = DWord2Float(part);
									 }

									 if (textlen > 0) 
									 {
										 allinps[segnum].t[ntext].text[0]='\0';
										 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
										 if (textlen<=20) memcpy(&allinps[segnum].t[ntext].text[0], &buf[0], textlen+1);
										 allinps[segnum].t[ntext].text[20]='\0';
										 if (!oldversion) GetAlf2Rus((unsigned char*)&allinps[segnum].t[ntext].text[0]);
										 else GetAlf2RusOld((unsigned char*)&allinps[segnum].t[ntext].text[0]);

										 allinps[segnum].t[ntext].y = textpos[1];

										 if (alignhor==0x72)							// 72 - выравнивание слева от точки
											{allinps[segnum].t[ntext].x = textpos[0]-2.1f*textlen;}			
										 else if (alignhor==0x6D)						// 6D - выравнивание по центру от точки
											{allinps[segnum].t[ntext].x = textpos[0]-1.05f*textlen;}		
										 else											// 6C - выравнивание справа от точки	
											{allinps[segnum].t[ntext].x = textpos[0];}					

										 if (allinps[segnum].t[ntext].text[0]=='n') // УРА! КРУЖОЧЕК! - инверсный вход
										 {
											 allinps[segnum].negin=true;
										 }
										 //String^ tog = gcnew String((char*)buf);
		//								 g1->DrawString(tog, font1, System::Drawing::Brushes::Black, w/2+mas*textpos[0]+10+ntext*20, h/2-mas*textpos[1]-font1->SizeInPoints -mas+1);
										 if (visible==0x6A) allinps[segnum].t[ntext].vis = 1;

										 if (ntext<7) ntext++;
									 }
									 //if (tt%2==1) fread(&t1,1,1,bigin); 
									 tubi = 0x00000000;
								 }
								 else if ((tubi & 0x0000FFFF) == 0x00006B72)		// а это инверсный выход
								 {
									 std::fread((void*)buf,1,6,bigin);
									 for (int j=0; j<4; j++)
									 {
										 unsigned char part[4];
										 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
										 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
										 allinps[segnum].circlepos[j] = DWord2Float(part);
									 }
									 allinps[segnum].negout=true;
									 tubi = 0x00000000;
								 }
							 } ;

							 std::fsetpos(bigin,&fpos1);

												// поиск родителя входа/выхода. С каким ubi он связан?
							 do {
								 std::fread(&t1,1,1,bigin);
								tubi = (tubi << 8) + t1;
								if ((tubi==0x030AF30F || tubi==0x0A03F30F) ) // parent ubi
								{
									for (int i=0; i<4; i++) 
									{
										std::fread(&t1,1,1,bigin);
										tubi = (tubi << 8) + t1;
									}
									if (oldversion) allinps[segnum].parent = (tubi & 0x000000FF);
									else allinps[segnum].parent = (tubi & 0xFF000000)>>24;
								}
								else if ((tubi==0x030A730F || tubi==0x0A03730F) ) // link line
								{
									 for (int i=0; i<4; i++) 
									 {
										 std::fread(&t1,1,1,bigin);
										tubi = (tubi << 8) + t1;
									 }
									 if (oldversion) allinps[segnum].linelink = (tubi & 0x000000FF);
									 else allinps[segnum].linelink = (tubi & 0xFF000000)>>24;
								}
							 } while (((tubi & 0x0000FFFF) != 0x00000106 && (tubi & 0x0000FFFF) != 0x00000601) && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
							 if (((tubi & 0x0000FFFF) == 0x00000106) || ((tubi & 0x0000FFFF) == 0x00000601)) 
							 {
								 std::fgetpos(bigin,&fpos1);
								fpos1-=8; 
								std::fsetpos(bigin,&fpos1);
							 } 
						 }
						 else if ( (tubi & 0x00FFFFFF) == 0x0000000A && segnum <= nCONS ) // connector
						 {
							 do
							 {	
								 std::fread(&t1,1,1,bigin);
								 tubi = (tubi << 8) + t1;
							 } while (tubi!=0x04060500 && tubi!=0x04060005 && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
							 float conpos[2] = {0,0};		//conpos
							 for (int j=0; j<2; j++)
							 {
								 unsigned char part[4];
								 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
								 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
								 conpos[j] = DWord2Float(part);
							 }
							 allcons[segnum].num = segnum;
							 allcons[segnum].pos.x  = conpos[0]; 
							 allcons[segnum].pos.y  = conpos[1];

							 //pen1->Brush = Brushes::Blue;
							 //g1->DrawLine(pen1, w/2+mas*conpos[0]+3, h/2-mas*conpos[1]+3, w/2+mas*conpos[0]-3, h/2-mas*conpos[1]-3);
							 //g1->DrawLine(pen1, w/2+mas*conpos[0]-3, h/2-mas*conpos[1]+3, w/2+mas*conpos[0]+3, h/2-mas*conpos[1]-3);
	//						 g1->DrawString(Convert::ToString((unsigned char)segnum), font1, System::Drawing::Brushes::Blue, w/2+mas*conpos[0], h/2-mas*(conpos[1]+1)-font1->SizeInPoints -mas+1);
							 pen1->Brush = Brushes::Black;

							 std::fread((void*)buf,1,22,bigin);

							 for (int i=0; i<4; i++) 
							 {
								 std::fread(&t1,1,1,bigin);
								 tubi = (tubi << 8) + t1;
							 }

							 unsigned char textlen=0;		// conname
							 unsigned char alignhor=0;
							 long tt = ((tubi & 0x00FF0000) >> 16) - 4;
							 if (oldversion) tt = ((tubi & 0xFF000000) >> 24) - 4;

							 std::fread((void*)buf,1,3,bigin);
							 std::fread((void*)&alignhor,1,1,bigin);
							 tt-=4;
							 do
							 {
								 if (t1!=0x00)
								 {
									 std::fread(&t1,1,1,bigin);
									tubi = (tubi << 8) + t1;
								 }
								 else
								 {
									 std::fread(&t1,1,1,bigin);
								 }
								 tt--;
							 } while ((tubi&0xFFFF00FF)!=0x9999005E && (tubi&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
							 textlen = tt-8;			

							 float connamepos[2] = {0,0};
							 for (int j=0; j<2; j++)
							 {
								 unsigned char part[4];
								 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
								 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
								 connamepos[j] = DWord2Float(part);
							 }

							 String^ ubiname = "";
							 String^ conname = "";
							 if (textlen>0)
							 {
								 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
								 if (textlen<=20) memcpy(&allcons[segnum].conname.text[0],&buf[0],textlen+1); 

								 if (alignhor==0x72) {connamepos[0]-=2.0f*textlen;}			// 6C - выравнивание справа от точки
								 else if (alignhor==0x6D)	{connamepos[0]-=textlen;}		// 6D - выравнивание по центру от точки
																							// 72 - выравнивание слева от точки
							 
								 conname = gcnew String((char*)buf);
								 allcons[segnum].conname.vis = 1;
							 }
							 //if (textlen%2==1) {std::fread(&t1,1,1,bigin);} 
						 
							 do 
							 {
								 std::fread(&t1,1,1,bigin);
								 tubi = (tubi << 8) + t1;
							 } while ((tubi & 0xFF00FFFF) != 0x0200060C && (tubi & 0x00FFFFFF) != 0x0002060C && tubi!=0x0702000C && tubi!=0x0207000C && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));

							 if ((tubi & 0xFF00FFFF) == 0x0200060C || (tubi & 0x00FFFFFF) == 0x0002060C) // con1
							 {
								 textlen=0;		
								 alignhor=0;
								 tt = ((tubi & 0x00FF0000) >> 16) - 4;
								 if (oldversion) tt = ((tubi & 0xFF000000) >> 24) - 4;
								 std::fread((void*)buf,1,3,bigin); 
								 std::fread((void*)&alignhor,1,1,bigin);
								 tt-=4;
								 do
								 {
									 if (t1!=0x00)
									 {
										std::fread(&t1,1,1,bigin);
										tubi = (tubi << 8) + t1;
									 }
									 else
									 {
										std::fread(&t1,1,1,bigin);
									 }
									 tt--;
								 } while ((tubi&0xFFFF00FF)!=0x9999005E && (tubi&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
								 textlen = tt-8;

								 float textpos[2] = {0,0};
								 for (int j=0; j<2; j++)
								 {
									 unsigned char part[4];
									 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
									 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
									 textpos[j] = DWord2Float(part);
								 }
								 if (textlen > 0) 
								 {
									 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
									 //String^ tog = gcnew String((char*)buf);
									 //g1->DrawString(tog, font1, System::Drawing::Brushes::Black, w/2+mas*textpos[0], h/2-mas*textpos[1]-font1->SizeInPoints -mas+1);
									 allcons[segnum].cons[0].x = textpos[0];
									 allcons[segnum].cons[0].y = textpos[1];
									 memcpy(&allcons[segnum].cons[0].text[0],&buf[0],10); 

									 nacts++;	// add con to acts
									 allacts[nacts].num = nacts;
									 allacts[nacts].zone.x1 = textpos[0];
									 allacts[nacts].zone.y1 = textpos[1];
									 allacts[nacts].zone.x2 = textpos[0]+textlen*2.1f;
									 allacts[nacts].zone.y2 = textpos[1]+3.0f;
									 memcpy(&allacts[nacts].list[0],&buf[0],3);
								 }
								 //if (textlen%2==1) {std::fread(&t1,1,1,bigin);} 

								 //
								 unsigned char ncons=1;
								 templong = 0x00000000;
								 while (!std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) )
								 {
									 std::fread(&t1,1,1,bigin);
									 templong = (templong << 8) + t1;

									 if ((templong == 0x0207000C || templong == 0x0702000C) && allcons[segnum].slashtype[0]=='\0') // slash type
									 {
										 templong = 0x00000000;
										 long ubinamelen=0;	 
										 do
										 {
											 std::fread(&t1,1,1,bigin);
											 templong = (templong << 8) + t1;
										 } while ((templong & 0xFF00FFFF) != 0x0200000C && (templong & 0x00FFFFFF) != 0x0002000C && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
									 
										 //if ((templong & 0xFF00FFFF) == 0x0200000C && (templong & 0x00FFFFFF) != 0x0002000C)

										 if (oldversion)  ubinamelen = ((templong & 0xFF000000)>>24) - 0x06;
										 else ubinamelen = ((templong & 0x00FF0000)>>16) - 0x06;
										 std::fread((void*)buf,1,2,bigin);
										 if (ubinamelen<21) 
										 {
											std::fread((void*)&allcons[segnum].slashtype[0],1,ubinamelen,bigin); 
											allcons[segnum].slashtype[ubinamelen]='\0';
											//String^ tog22 = gcnew String((char*)allcons[segnum].slashtype);
										 }
										 templong = 0x00000000;
										 std::fgetpos(bigin,&segcurpos);
										 if ( segcurpos+42 < segendpos-4) std::fread((void*)buf,1,42,bigin);
										 //if (ubinamelen%2==1) {std::fread(&t1,1,1,bigin);}
										 //std::fread((void*)buf,1,18,bigin);  //usedbaits-=26;
										 //g1->DrawString(Convert::ToString((float)usedbaits), font1, System::Drawing::Brushes::Black, w/2+mas*textpos[0], h/2-mas*(textpos[1]+5)-font1->SizeInPoints -mas+1);
									 }
								 
									 else if ((templong & 0xFF00FFFF) == 0x0200060C || (templong & 0x00FFFFFF) == 0x0002060C) // conN...
									 {
										 textlen=0;		
										 alignhor=0;
										 tt = ((templong & 0x00FF0000) >> 16) - 4;
										 if (oldversion) tt = ((templong & 0xFF000000) >> 24) - 4;

										 textlen = 0x0;
										 std::fread((void*)buf,1,3,bigin); 
										 std::fread((void*)&alignhor,1,1,bigin);
										 tt-=4;

										 do
										 {
											 if (t1!=0x00)
											 {
												std::fread(&t1,1,1,bigin);
												templong = (templong << 8) + t1;
											 }
											 else
											 {
												std::fread(&t1,1,1,bigin);
											 }
											 tt--;
										 } while ((templong&0xFFFF00FF)!=0x9999005E && (templong&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4));
										 textlen = tt-8;

										 float textpos[2] = {0,0};
										 for (int j=0; j<2; j++)
										 {
											 unsigned char part[4];
											 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
											 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
											 textpos[j] = DWord2Float(part);
										 }
										 if (textlen > 0) 
										 {
											 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
											 //String^ tog3 = gcnew String((char*)buf);
											 //g1->DrawString(tog, font1, System::Drawing::Brushes::Black, w/2+mas*textpos[0], h/2-mas*textpos[1]-font1->SizeInPoints -mas+1);
											 allcons[segnum].cons[ncons].x = textpos[0];
											 allcons[segnum].cons[ncons].y = textpos[1];
											 memcpy(&allcons[segnum].cons[ncons].text[0],&buf[0],10);
								 
											 nacts++;	// add Con to acts
											 allacts[nacts].num = nacts;
											 allacts[nacts].zone.x1 = textpos[0];
											 allacts[nacts].zone.y1 = textpos[1];
											 allacts[nacts].zone.x2 = textpos[0]+textlen*2.1f;
											 allacts[nacts].zone.y2 = textpos[1]+3.0f;
											 memcpy(&allacts[nacts].list[0],&buf[0],3);
											 allacts[nacts].kks[0]='\0';

											 if (ncons<9) ncons++;
										 }
										 //if (t2%2==1) std::fread(&t1,1,1,bigin);
									 }
									 else if (templong==0x030A620F || templong==0x0A03620F) // принадлежность punkt
									 {
										 unsigned long templong2=0x00000000;
										 for (int q2=0; q2<4; q2++) {std::fread(&t1,1,1,bigin); templong2 = (templong2 << 8) + t1;}
										 if (oldversion) allcons[segnum].punlink = (templong2 & 0x000000FF);
										 else allcons[segnum].punlink = ((templong2 & 0xFF000000) >> 24);
									 }
								 };

							 }
							 else if (tubi == 0x0207000C || tubi == 0x0702000C)  // empty slash
							 {
								 templong = 0x00000000;
								 long ubinamelen=0;

								 do
								 {
									 std::fread(&t1,1,1,bigin);
									 templong = (templong << 8) + t1;
								 } while ((templong & 0xFF00FFFF) != 0x0200000C && (templong & 0x00FFFFFF) != 0x0002000C && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) );

								 if (oldversion) ubinamelen = ((templong & 0xFF000000)>>24) - 6;
								 else ubinamelen = ((templong & 0x00FF0000)>>16) - 6;
								 std::fread((void*)buf,1,2,bigin); 
								 if (ubinamelen<21) 
								 {
									std::fread((void*)&buf[0],1,ubinamelen,bigin); 
									buf[ubinamelen]='\0';
								 }
								 //if (ubinamelen%2==1) std::fread(&t1,1,1,bigin);

								 ubiname = gcnew String((const char*)&buf[0]);
								 allcons[segnum].slashtype[0] = '\0';  
							 }

							 allcons[segnum].conname.x = connamepos[0];
							 allcons[segnum].conname.y = connamepos[1]; 

						 }
						 else if ((tubi & 0x00FFFFFF) == 0x00000008 && segnum <= nOBJS) // -------------------- ubi.object --------------------------- 
						 {
							 //HRSRC hRes=;
							 //if(!hRes){continue;}
							 String^ ubiname = gcnew String((const char*)&segname[0]);//+".sg";
							 if (FindResourceA(hMod,(LPCSTR)segname,(LPCSTR)RT_RCDATA)==NULL)
							 {
								 if (ubiname->Contains("rahmen0")) ubiname="ubi.rahmen0";
								 else if (ubiname->Contains("rahmen11")) ubiname="ubi.rahmen11";
								 else if (ubiname->Contains("rahmen21")) ubiname="ubi.rahmen21";
								 else if (ubiname->Contains("rahmen1")) ubiname="ubi.rahmen1";
								 else if (ubiname->Contains("rahmen2")) ubiname="ubi.rahmen2";
								 else if (ubiname->Contains("untpaa")) ubiname="ubi.paa";

								 //ubiname=ubiname->Replace(".rb.",".")->Replace(".rs.",".")->Replace(".ren.",".")->Replace(".srz.",".")->Replace(".krz.",".");
								 //ubiname=ubiname->Replace(".aet.",".")->Replace(".asr.",".")->Replace(".bsr.",".")->Replace(".fgs.",".")->Replace(".stg.",".");
								 //ubiname=ubiname->Replace(".rkm.",".")->Replace(".fre.",".")->Replace(".mpa.",".")->Replace(".rba.",".")->Replace(".fkm.",".");
								 //ubiname=ubiname->Replace(".rbb.",".")->Replace(".rbd.",".")->Replace(".munt.",".")->Replace(".zie.",".")->Replace(".rb2.",".");
								 //ubiname=ubiname->Replace(".bsr2.",".");
								 char* temp = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ubiname).ToPointer();
								 memcpy(&segname[0],temp,25);
							 }
							 if (FindResourceA(hMod,(LPCSTR)segname,(LPCSTR)RT_RCDATA)==NULL)
							 {
								 ubiname=ubiname->Replace("1","")->Replace("2","")->Replace("3","")->Replace("4","")->Replace("5","");
								 ubiname=ubiname->Replace("6","")->Replace("7","")->Replace("8","")->Replace("9","")->Replace("0","");
								 char* temp = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(ubiname).ToPointer();
								 memcpy(&segname[0],temp,25);
							 }
							 if (FindResourceA(hMod,(LPCSTR)segname,(LPCSTR)RT_RCDATA)!=NULL ) // Accha
							 {
								 unsigned long indic = 0x00000000;
								 //long p=1;
								 long ntext=0;
								 do
								 {
									std::fread(&t1,1,1,bigin);
									indic = (indic << 8) + t1;

									 if ( ((indic & 0x0000FFFF) == 0x0500 || (indic & 0x0000FFFF) == 0x0005) && allobjs[segnum].num==0 ) //pos
									 {
										 float point[2] = {0,0};
										 for (int j=0; j<2; j++)
										 {
											 unsigned char pos[4];
											 if (oldversion) for (int h=0; h<4; h++) std::fread(&pos[h],1,1,bigin);
											 else for (int h=0; h<4; h++) std::fread(&pos[3-h],1,1,bigin);
											 point[j] = DWord2Float(pos);
										 }
										 this->listBox2->Items->Add("Ubi"+Convert::ToString((long)segnum)+"=["+ubiname+"] Pos=["+Convert::ToString(point[0])+","+Convert::ToString(point[1])+"]");
										 //DrawElf(this->listBox1->Items->IndexOf(ubiname), 0, point[0], point[1]);
										 memcpy(&allobjs[segnum].type[0],&segname[0],segnamelen+1);
										 allobjs[segnum].num = segnum;
										 allobjs[segnum].color = 0;
										 allobjs[segnum].pos.x  = point[0];
										 allobjs[segnum].pos.y  = point[1];
										 indic = 0x00000000;
									 }
									 else if ( indic == 0x04061500 || indic == 0x04060015) //rect
									 {
										 std::fread((void*)buf,1,16,bigin);
										 indic = 0x00000000;
									 }
									 else if ( (indic & 0xFF00FFFF) == 0x0200060C || (indic & 0x00FFFFFF) == 0x0002060C) //нашелся текст рядом с ubi
									 {
										 long tt = ((indic & 0x00FF0000) >> 16) - 4;
										 if (oldversion) tt = ((indic & 0xFF000000) >> 24) - 4; //0x22; // 0x25; в бэкпах старше Кудан-Кулам	
										 long textlen=0; 	
										 unsigned char align=0;
										 unsigned char visible=0;

										 std::fread((void*)buf,1,2,bigin); 
										 std::fread((void*)&visible,1,1,bigin);	
										 std::fread((void*)&align,1,1,bigin);
										 tt-=4;

										 do
										 {
											 if (t1!=0x00) 
											 {
												 std::fread(&t1,1,1,bigin);
												 indic = (indic << 8) + t1;
											 }
											 else
											 {
												std::fread(&t1,1,1,bigin);
											 }
											 tt--; 
										 } while ((indic&0xFFFF00FF)!=0x9999005E && (indic&0xFFFF00FF)!=0x5E5E005E && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) && !feof(bigin));
										 textlen = tt-8;
								 
										 float textpos[2] = {0,0};
										 for (int j=0; j<2; j++)
										 {
											 unsigned char part[4];
											 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
											 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
											 textpos[j] = DWord2Float(part);
										 }

										 if (textlen >= 0) 
										 {
											 allobjs[segnum].t[ntext].text[0]='\0';
											 std::fread((void*)buf,1,textlen,bigin); buf[textlen]='\0';
											 String^ tog33 = gcnew String((char*)&buf[0]);

											 if (textlen<=20) memcpy(&allobjs[segnum].t[ntext].text[0], &buf[0], textlen+1); 
											 allobjs[segnum].t[ntext].text[20]='\0';

											 long pointpos=-1;
											 for (int i=0; i<textlen; i++) if (buf[i]=='.') pointpos=i;
											 if (pointpos>=0) pointpos=textlen-pointpos;
											 else pointpos=0;

											 allobjs[segnum].t[ntext].y = textpos[1];
											 if (align==0x72)							// 72 - выравнивание слева от точки
												{allobjs[segnum].t[ntext].x = textpos[0]-2.1f*textlen+1.05f*pointpos;}			
											 else if (align==0x6D)						// 6D - выравнивание по центру от точки
												{allobjs[segnum].t[ntext].x = textpos[0]-1.05f*textlen+1.05f*pointpos;}		
											 else										// 6C - выравнивание справа от точки	
												{allobjs[segnum].t[ntext].x = textpos[0];}					

											 if (visible==0x6A) allobjs[segnum].t[ntext].vis = 1;

											 if (allobjs[segnum].t[ntext].text[0]=='B' && allobjs[segnum].t[ntext].text[1]=='G') {allobjs[segnum].t[ntext].text[0]='\0'; ntext--;}
											 else if (allobjs[segnum].t[ntext].text[0]=='b' && allobjs[segnum].t[ntext].text[1]=='g') {allobjs[segnum].t[ntext].text[0]='\0'; ntext--;}
											 else if (allobjs[segnum].t[ntext].text[0]=='0') {allobjs[segnum].t[ntext].text[0]='\0'; ntext--;}
											 //else if (allobjs[segnum].t[ntext].text[0]=='1' && textlen==1) {allobjs[segnum].t[ntext].text[0]='\0'; ntext--;}
										 
											 if (ntext<15) ntext++;
											 //seglen-=textlen;
										 }
										 //if (tt%2==1) {std::fread(&t1,1,1,bigin);} 
										 indic = 0x00000000;
									 }
									 else if (indic == 0x030AA20F || indic == 0x0A03A20F) // связанные с ubi входы/выходы
									 {
										 indic = 0x00000000;
										 std::fsetpos(bigin,&segendpos);
									 }
									 else if (indic == 0x0207000C || indic == 0x0702000C) // свойства названия элемента / конец описания
									 {
										 indic = 0x00000000;
										 std::fsetpos(bigin,&segendpos);
									 }

								 //p++;
								 } while (!std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) && !feof(bigin) );

							 }
							 else
							 {
								 this->listBox2->Items->Add(ubiname);
							 }
						 }

					 }

				 } while (  !feof( bigin )   /*(t1!='EOF') && n<200000*/);
				else {MessageBox::Show("Fail! (*bigin)");}


				for (unsigned long q1=1; q1<=nPUNS; q1++) 
				if (punsadd[q1]!=0)
				{
					unsigned char buf2[6];
					fpos_t punpos=0;
					fpos_t segendpos=0;
					fpos_t segcurpos=0;
					punpos=fposb+2*(punsadd[q1]-1);
					std::fsetpos(bigin,&punpos);

					std::fread((void*)buf2,1,4,bigin);
					unsigned long seglen = (buf2[1]<<8) + buf2[0];
					if (oldversion) seglen = (buf2[0]<<8) + buf2[1];
					 
					segendpos = punpos + seglen*2;
				
					tubi = 0x00000000;
					for (int i=0; i<4; i++) 
					{
						std::fread(&t1,1,1,bigin); 
						if (oldversion) tubi = tubi | (t1 << (i*8));
						else tubi = (tubi << 8) + t1;
					}
					unsigned long sn = (tubi & 0xFF000000)>>24;

					if ( (tubi & 0x00FFFFFF) == 0x00000002 && sn <= nPUNS) // punkt
					{
						 std::memset(&allpuns[sn],0,sizeof(PUNS));

						 std::fread((void*)buf2,1,6,bigin);
						 float point[2] = {0,0};
						 for (int j=0; j<2; j++)
						 {
							 unsigned char part[4];
							 if (oldversion) for (int i=0; i<4; i++) std::fread(&part[i],1,1,bigin);
							 else for (int i=0; i<4; i++) std::fread(&part[3-i],1,1,bigin);
							 point[j] = DWord2Float(part);
						 }
						 allpuns[sn].num = sn;
						 allpuns[sn].color = 0;
						 allpuns[sn].pos.x = point[0];
						 allpuns[sn].pos.y = point[1];

						 tubi=0x00000000;
						 unsigned char nparent=0;
					 
						 do
						 {
							 std::fread(&t1,1,1,bigin); 
							 tubi = (tubi << 8) + t1;
							 if (tubi==0x020A0004 || tubi==0x0A020004)  allpuns[sn].sq = true; // квадратик
							 else if (tubi==0x030A730F || tubi==0x030A430F || tubi==0x0A03730F || tubi==0x0A03430F) // принадлежность линии или коннектору
							 {
								 std::fread((void*)buf2,1,4,bigin);
								 if (oldversion) {allpuns[sn].nlink[nparent] = buf2[3]; allpuns[sn].tlink[nparent] = buf2[0];}
								 else {allpuns[sn].nlink[nparent] = buf2[0]; allpuns[sn].tlink[nparent] = buf2[3]; }
								 if (nparent<3) nparent++;
							 }
						 } while (!std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) && !feof(bigin));
					}
			
				}


				for (unsigned long q1=1; q1<=nLINS; q1++) 
				if (linsadd[q1]!=0)
				{
					//this->listBox2->Items->Add("Lin"+Convert::ToString((long)q1)+ " add= "+Convert::ToString((long)linsadd[q1]));

					unsigned char buf3[4];
					fpos_t punpos=0;
					fpos_t linpos=0;
					fpos_t segendpos=0;
					fpos_t segcurpos=0;
					linpos=fposb+2*(linsadd[q1]-1);
					std::fsetpos(bigin,&linpos);

					std::fread((void*)buf3,1,4,bigin);
					unsigned long seglen = (buf3[1]<<8) + buf3[0];
					if (oldversion) seglen = (buf3[0]<<8) + buf3[1];
				
					segendpos = linpos + seglen*2;

					unsigned long tubili=0x00000000;
					for (int i=0; i<4; i++) 
					{
						std::fread(&t1,1,1,bigin); 
						if (oldversion) tubili = tubili | (t1 << (i*8));
						else tubili = (tubili << 8) + t1;
					}

					unsigned long snli = (tubili & 0xFF000000)>>24;

					if ((tubili & 0x00FFFFFF) == 0x00000003 && snli <= nLINS) // line
					{
						std::memset(&alllins[snli],0,sizeof(LINS));
						alllins[snli].num = snli;
						alllins[snli].color = 0;

						 if ((seglen+4)/2>0x0E) alllins[snli].type = true;
 
						 String^ s="Line " +Convert::ToString((float)snli)+ " =[";
						 tubili=0x00000000;
					 
						 while (tubili!=0x030A220F && tubili!=0x0A03220F && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) && !feof(bigin) )
						 {
							 std::fread(&t1,1,1,bigin);
							 tubili = (tubili << 8) + t1;
						 }

						 if (tubili==0x030A220F || tubili==0x0A03220F)
						 {
							 std::fread((void*)buf3,1,4,bigin);
							 if (oldversion) 
							 {
								 alllins[snli].nobj[0] = (buf3[2]<<8) + buf3[3]; s+=Convert::ToString((buf3[2]<<8) + buf3[3])+",";
								 alllins[snli].tobj[0] = buf3[0]; s+=Convert::ToString(buf3[0])+"]-[";
							 }
							 else
							 {
								 alllins[snli].nobj[0] = (buf3[1]<<8) + buf3[0]; s+=Convert::ToString((buf3[1]<<8) + buf3[0])+",";
								 alllins[snli].tobj[0] = buf3[3]; s+=Convert::ToString(buf3[3])+"]-[";
							 }
							 tubili=0x00000000;
						 }

						 while (tubili!=0x030A220F && tubili!=0x0A03220F && !std::fgetpos(bigin,&segcurpos) && segcurpos<(segendpos-4) )
						 {
							 std::fread(&t1,1,1,bigin);
							 tubili = (tubili << 8) + t1;
						 }

						 if (tubili==0x030A220F || tubili==0x0A03220F)
						 {
							 std::fread((void*)buf3,1,4,bigin);
							 if (oldversion) 
							 {
								 alllins[snli].nobj[1] = (buf3[2]<<8) + buf3[3]; s+=Convert::ToString((buf3[2]<<8) + buf3[3])+",";
								 alllins[snli].tobj[1] = buf3[0]; s+=Convert::ToString(buf3[0])+"]";
							 }
							 else
							 {
								 alllins[snli].nobj[1] = (buf3[1]<<8) + buf3[0]; s+=Convert::ToString((buf3[1]<<8) + buf3[0])+",";
								 alllins[snli].tobj[1] = buf3[3]; s+=Convert::ToString(buf3[3])+"]";
							 }
						 }

						this->listBox2->Items->Add(s);
					}
				}

				if (bigin!=NULL) std::fclose(bigin);

			
				}

		// ------------------------ прорисовка всех объектов ---------------------------------
			
		String^ awe_fstr = gcnew String((char*)info.awenum);
		String^ bgtype_fstr = gcnew String((char*)info.bgtype);
		String^ bgnum_fstr = gcnew String((char*)info.bgnum);

		/*char *outfilename;
		FILE* out2;
		outfilename = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->toolStripTextBox2->Text+this->SearchResults->SelectedItem+".txt").ToPointer();
		out2=fopen(outfilename,"r");
		if (out2==NULL) 
		{
			FILE* out;
			//if (test) out=fopen(outfilename,"w");
			if (out!=NULL)
			{*/
				for (unsigned long q1=1; q1<=nOBJS; q1++)		// прорисовка всех объектов ubi
				if (allobjs[q1].num!=0 /*&& allobjs[q1].type>0 && allobjs[q1].type < this->listBox1->Items->Count*/)
				{
					DrawElf(g1,&allobjs[q1].type[0], allobjs[q1].pos.x, allobjs[q1].pos.y);
					/*if (allobjs[q1].type[4]=='d' && allobjs[q1].type[5]=='e') //на листе есть блочек с дельтой
					{
							fprintf(out,"Hello, World!\n");
							if (test) fprintf(out,"DELT%s\nspo : %.2f %.2f; loe\n" , allobjs[q1].t[0].text , allobjs[q1].pos.x, allobjs[q1].pos.y );
							String^ DE="";
							String^ DA="";

							if (test) for (unsigned long q2=1; q2<=nINPS; q2++)	// заполнение выходного файла
							if (allinps[q2].num!=0 && allinps[q2].parent==q1 && allinps[q2].link==true)
							{
								if (allinps[q2].name[1] == 'E') DE = Convert::ToString(allinps[q2].pos[0].x) + " " + Convert::ToString(allinps[q2].pos[0].y);
								else if (allinps[q2].name[1] == 'A') DA = Convert::ToString(allinps[q2].pos[0].x) + " " + Convert::ToString(allinps[q2].pos[0].y);
								else if (allinps[q2].name[1] == 'W')
								{
									long p1=FindPartner(q2,out);
									if (p1>=0) 
									{
										if (test) fprintf(out,"spo : %.2f %.2f; loe\n", allobjs[allinps[p1].parent].pos.x, allobjs[allinps[p1].parent].pos.y );
										for (unsigned long q3=1; q3<=nINPS; q3++)
										if (allinps[q3].num!=0 && allinps[q3].parent==allinps[p1].parent && allinps[q3].link==true && q3!=p1)
										{
											//fprintf(out,"%s: pos = [%.2f, %.2f]\n", allinps[q3].name , allinps[q3].pos[0].x, allinps[q3].pos[0].y );
											long p2=FindPartner(q3,out);
											//if (p2>=0) fprintf(out,"%d, %s: pos = [%.2f, %.2f]\n", p2+57, allinps[p2].name , allinps[p2].pos[0].x, allinps[p2].pos[0].y );
											if (p2>=0) if (test) fprintf(out,"spo : %.2f %.2f; loe\n", allobjs[allinps[p2].parent].pos.x, allobjs[allinps[p2].parent].pos.y );
										}
									}
								}
								else if (allinps[q2].name[1] == 'P')
								{
									long p1=FindPartner(q2,out);
									if (p1>=0) 
									{
										if (test) fprintf(out,"spo : %.2f %.2f; loe\n", allobjs[allinps[p1].parent].pos.x, allobjs[allinps[p1].parent].pos.y );
										for (unsigned long q3=1; q3<=nINPS; q3++)
										if (allinps[q3].num!=0 && allinps[q3].parent==allinps[p1].parent && allinps[q3].link==true && q3!=p1)
										{
											//fprintf(out,"%s: pos = [%.2f, %.2f]\n", allinps[q3].name , allinps[q3].pos[0].x, allinps[q3].pos[0].y );
											long p2=FindPartner(q3,out);
											//if (p2>=0) fprintf(out,"%d, %s: pos = [%.2f, %.2f]\n", p2+57, allinps[p2].name , allinps[p2].pos[0].x, allinps[p2].pos[0].y );
											if (p2>=0) if (test) fprintf(out,"spo : %.2f %.2f; loe\n", allobjs[allinps[p2].parent].pos.x, allobjs[allinps[p2].parent].pos.y );
										}
									}
								}
								else {} //fail!!
							}

							char *tout = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(DE->Replace(",",".")+" "+DA->Replace(",",".")).ToPointer();
							if (test) fprintf(out,"vlf : %s\n" , tout);
					}*/

					//font1->FontFamily =  = gcnew System::Drawing::Font("Microsoft Sans Serif",2.0f*mas);
					for (int q2=0; q2<8; q2++) // прорисовка всех видимых свойств ubi
					if (allobjs[q1].t[q2].text[0]!='\0' )
					{
						if (strstr(allobjs[q1].type,"rahmen")!=NULL)
						{
							String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
							if (allobjs[q1].t[q2].vis==1) g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*(allobjs[q1].t[q2].x), h/2-mas*allobjs[q1].t[q2].y -font1->SizeInPoints -mas+1);
						
							if (allobjs[q1].t[q2].text[0]=='-') 
							{
								String^ tog2 = gcnew String(info.bpotype);
								g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*(allobjs[q1].t[q2].x+2.0f), h/2-mas*allobjs[q1].t[q2].y -font1->SizeInPoints-mas+1);
							}
						}
						else if (strstr(allobjs[q1].type,"abfsys")!=NULL)
						{
							String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
							if (allobjs[q1].t[q2].vis==1) g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*(allobjs[q1].t[q2].x), h/2-mas*allobjs[q1].t[q2].y -font1->SizeInPoints -mas+1);
						}
						else
						{
							String^ tog2 = gcnew String((char*)allobjs[q1].t[q2].text);
							if (allobjs[q1].t[q2].vis==1)
							{
								g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*(allobjs[q1].t[q2].x), h/2-mas*allobjs[q1].t[q2].y -font1->SizeInPoints -mas+1);
					
							}
						}

					}
				}
			/*}
			if (out!=NULL) fclose(out);
		}
		if (out2!=NULL) std::fclose(out2);*/


				for (unsigned long q1=1; q1<=nINPS; q1++) allinps[q1].link = false;  

				pen1->Width = mas*0.35f;
				for (unsigned long q1=1; q1<=nINPS; q1++)	// прорисовка всех входов/выходов
				if (allinps[q1].num!=0)
				{
					String^ tog2="";
					pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;

					unsigned char nvis=0;
					for (int k=0; k<8; k++) if (allinps[q1].t[k].vis==1) nvis++;

					for (int k=0; k<8; k++)
					if (allinps[q1].t[k].vis==1) // рисовать только видимые надписи
					{
						if (nvis<=2 && (allinps[q1].pos[0].x!=allobjs[allinps[q1].parent].pos.x || allinps[q1].pos[0].y!=allobjs[allinps[q1].parent].pos.y || allinps[q1].name[0]=='Z' || allinps[q1].t[k].text[0]=='E') )
						{
							String^ tog2 = gcnew String((char*)allinps[q1].t[k].text);

							if (this->timer2->Enabled && tog2!="")
							{
								if (allinps[q1].t[k].text[0]=='M' || allinps[q1].t[k].text[0]=='E' || allinps[q1].t[k].text[0]=='T' || allinps[q1].t[k].text[0]=='A')
								{
									if (allinps[q1].t[k].text[1]!='O') 
									{
										tog2 = tog2->Replace("M,","M")->Replace("E,","E")->Replace("T,","T")->Replace("S,","S")->Replace("A,","A");
										String^ m_value = "";
										m_value = GetMarkerValue(awe_fstr, bgnum_fstr+","+tog2, &allinps[q1].color);
										if (m_value!="")
										{
											tog2 += "="+m_value;
											deep=0; ColorPartners(9, q1);
										}
									}
								}
							}

							switch (allinps[q1].color)
							{
								case 0:
									g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*allinps[q1].t[k].x, h/2-mas*allinps[q1].t[k].y - 1.5f*font1->SizeInPoints +1);
									break;
								case 1:
									g1->DrawString(tog2, font1, System::Drawing::Brushes::Blue, w/2+mas*allinps[q1].t[k].x, h/2-mas*allinps[q1].t[k].y - 1.5f*font1->SizeInPoints +1);
									break;
								case 2:
									g1->DrawString(tog2, font1, System::Drawing::Brushes::LimeGreen, w/2+mas*allinps[q1].t[k].x, h/2-mas*allinps[q1].t[k].y - 1.5f*font1->SizeInPoints +1);
									break;
								case 3:
									g1->DrawString(tog2, font1, System::Drawing::Brushes::Magenta, w/2+mas*allinps[q1].t[k].x, h/2-mas*allinps[q1].t[k].y - 1.5f*font1->SizeInPoints +1);
									break;
								default:
									g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*allinps[q1].t[k].x, h/2-mas*allinps[q1].t[k].y - 1.5f*font1->SizeInPoints +1);
									break;
							}
						}
						else 
						{
							allinps[q1].t[k].vis=2;
						}
					}
					if (allinps[q1].num != 0 && this->debugInfoToolStripMenuItem->Checked ) g1->DrawString(Convert::ToString((unsigned char) allinps[q1].num), font1, System::Drawing::Brushes::Green, w/2+mas*allinps[q1].pos[0].x, h/2-mas*(allinps[q1].pos[0].y+1)-font1->SizeInPoints -mas+1);

					/*if (allinps[q1].name[0]!='\0')
					{
						font1 = gcnew System::Drawing::Font("Microsoft Sans Serif",1.0f*mas);
						tog2 = gcnew String((char*)allinps[q1].name);
						tog2+= " p=" + Convert::ToString((long)allinps[q1].parent);
						g1->DrawString(tog2, font1, System::Drawing::Brushes::Blue, w/2+mas*allinps[q1].pos[0].x+mas, h/2-mas*allinps[q1].pos[0].y + font1->SizeInPoints - mas+1);
					}*/

															// прорисовка линии входа/выхода
					switch (allinps[q1].color)
					{
						case 0:
							pen1->Color = Color::Black;
							break;
						case 1:
							pen1->Color = Color::Blue;
							break;
						case 2:
							pen1->Color = Color::LimeGreen;
							break;
						case 3:
							pen1->Color = Color::Magenta;
							break;
						default:
							pen1->Color = Color::Black;
							break;
					}

					if (allinps[q1].pos[1].x!=0 && allinps[q1].pos[1].y!=0) g1->DrawLine(pen1, w/2+mas*allinps[q1].pos[0].x, h/2-mas*allinps[q1].pos[0].y, w/2+mas*allinps[q1].pos[1].x, h/2-mas*allinps[q1].pos[1].y);
					if (allinps[q1].pos[2].x!=0 && allinps[q1].pos[2].y!=0) g1->DrawLine(pen1, w/2+mas*allinps[q1].pos[1].x, h/2-mas*allinps[q1].pos[1].y, w/2+mas*allinps[q1].pos[2].x, h/2-mas*allinps[q1].pos[2].y);
					if (allinps[q1].pos[3].x!=0 && allinps[q1].pos[3].y!=0) g1->DrawLine(pen1, w/2+mas*allinps[q1].pos[2].x, h/2-mas*allinps[q1].pos[2].y, w/2+mas*allinps[q1].pos[3].x, h/2-mas*allinps[q1].pos[3].y);
					if (allinps[q1].pos[4].x!=0 && allinps[q1].pos[4].y!=0) g1->DrawLine(pen1, w/2+mas*allinps[q1].pos[3].x, h/2-mas*allinps[q1].pos[3].y, w/2+mas*allinps[q1].pos[4].x, h/2-mas*allinps[q1].pos[4].y);
					if (allinps[q1].negout==true)
					{
						float radius=sqrt((allinps[q1].circlepos[0]-allinps[q1].circlepos[2])*(allinps[q1].circlepos[0]-allinps[q1].circlepos[2])+(allinps[q1].circlepos[1]-allinps[q1].circlepos[3])*(allinps[q1].circlepos[1]-allinps[q1].circlepos[3]));
						g1->DrawEllipse(pen1, w/2+mas*(allinps[q1].circlepos[0]-radius), h/2-mas*(allinps[q1].circlepos[1]+radius), mas*2.0f*radius, mas*2.0f*radius);
					}

				}

				pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
				pen1->Width = 0.35f*mas;
				pen1->Color = Color::Black;

				for (unsigned long q1=1; q1<=nLINS; q1++)			// прорисовка всех линий
				if (alllins[q1].num!=0) 
				{
					bool good=true;
					float point[4] = {0,0,0,0};
					long n0 = alllins[q1].nobj[0];
					long t0 = alllins[q1].tobj[0];
					long n1 = alllins[q1].nobj[1];
					long t1 = alllins[q1].tobj[1];
					bool neg0=false, neg1=false;

					pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
					pen1->Width = 0.35f*mas;
					pen1->Color = Color::Black;

					 switch (alllins[q1].color)
					 {
						case 0:
							pen1->Color = Color::Black;
							break;
						case 1:
							pen1->Color = Color::Blue;
							break;
						case 2:
							pen1->Color = Color::LimeGreen;
							break;
						case 3:
							pen1->Color = Color::Magenta;
							break;
						default:
							pen1->Color = Color::Black;
							break;
					 }

					if (t0==9 && alltabs[n0].num>0) // линия, выходящая из таблицы
					{
						point[0] = alltabs[n0].pos.x;
						point[1] = alltabs[n0].pos.y;
						pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						alllins[q1].type = 1;
						cli::array<Single>^temp0 = {2.0f,10.0f};
						pen1->DashPattern = temp0;
					}
					else if (t0==9 && allinps[n0].num>0) // линия, выходящая из элемента
					{
						if (!allinps[n0].link) allinps[n0].link = true;
						if (allinps[n0].negin) neg0=true;
						point[0] = allinps[n0].pos[0].x;
						point[1] = allinps[n0].pos[0].y;
					}
					else if (t0==2 && allpuns[n0].num>0) // линия, выходящая из точки
					{
						point[0] = allpuns[n0].pos.x;
						point[1] = allpuns[n0].pos.y;
					}
					else good=false;

					if (t1==9 && alltabs[n1].num>0) // линия, уходящая в таблицу
					{
						point[2] = alltabs[n1].pos.x;
						point[3] = alltabs[n1].pos.y;
						pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						alllins[q1].type = 1;
						cli::array<Single>^temp0 = {2.0f,10.0f};
						pen1->DashPattern = temp0;
					}
					else if (t1==9 && allinps[n1].num>0) // линия, уходящая в элемент
					{
						if (!allinps[n1].link) allinps[n1].link = true;
						if (allinps[n1].negin) neg1=true;
						point[2] = allinps[n1].pos[0].x;
						point[3] = allinps[n1].pos[0].y;
					}
					else if (t1==2 && allpuns[n1].num>0) // линия, уходящая в точку
					{
						point[2] = allpuns[n1].pos.x;
						point[3] = allpuns[n1].pos.y;
					}
					else good=false;

					if (good==1) 
					{
						alllins[q1].x1 = point[0];
						alllins[q1].y1 = point[1];
						alllins[q1].x2 = point[2];
						alllins[q1].y2 = point[3];
						float dcx=-0.9f,dcy=-0.9f;
						float inplen=5.0f;
						if (neg0)
						{
							if (fabs(point[0]-point[2])>1) dcx-=0.9f*(point[0]-point[2])/fabs(point[0]-point[2]);
							if (fabs(point[1]-point[3])>1) dcy+=0.9f*(point[1]-point[3])/fabs(point[1]-point[3]);
							g1->DrawEllipse(pen2, w/2+mas*(point[0]+dcx), h/2-mas*(point[1]-dcy), mas*1.8f,mas*1.8f);
							if (fabs(point[0]-point[2])>1) point[0]-=1.8f*(point[0]-point[2])/fabs(point[0]-point[2]);
							if (fabs(point[1]-point[3])>1) point[1]-=1.8f*(point[1]-point[3])/fabs(point[1]-point[3]);
							inplen=3.2f;
						}
						else if (neg1)
						{
							if (fabs(point[0]-point[2])>1) dcx+=0.9f*(point[0]-point[2])/fabs(point[0]-point[2]);
							if (fabs(point[1]-point[3])>1) dcy-=0.9f*(point[1]-point[3])/fabs(point[1]-point[3]);
							g1->DrawEllipse(pen2, w/2+mas*(point[2]+dcx), h/2-mas*(point[3]-dcy), mas*1.8f,mas*1.8f);
							if (fabs(point[0]-point[2])>1) point[2]+=1.8f*(point[0]-point[2])/fabs(point[0]-point[2]);
							if (fabs(point[1]-point[3])>1) point[3]+=1.8f*(point[1]-point[3])/fabs(point[1]-point[3]);
							inplen=3.2f;
						}
						g1->DrawLine(pen1, w/2+mas*point[0], h/2-mas*point[1], w/2+mas*point[2], h/2-mas*point[3]);
						if (t0==9 && alltabs[n0].num>0 && t1==9 && allinps[n1].num>0)
						{
							pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
							g1->DrawLine(pen1, w/2+mas*point[2], h/2-mas*point[3], w/2+mas*(point[2]-(point[2]-point[0])/fabs(point[2]-point[0])*inplen), h/2-mas*point[3]);
						}
						else if (t0==9 && allinps[n0].num>0 && t1==9 && alltabs[n1].num>0)
						{
							pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
							g1->DrawLine(pen1, w/2+mas*point[0], h/2-mas*point[1], w/2+mas*(point[0]-(point[0]-point[2])/fabs(point[0]-point[2])*inplen), h/2-mas*point[1]);
						}
					}
				}


				for (unsigned long q1=1; q1<=nCONS; q1++)   // прорисовка всех коннекторов
				if (allcons[q1].num!=0)
				{
					if (allcons[q1].slashtype[0]!='\0') DrawElf(g1, allcons[q1].slashtype, allcons[q1].pos.x, allcons[q1].pos.y);

					String^ conname_s = gcnew String((char*)allcons[q1].conname.text);
					String^ conname_mark = "";
					allcons[q1].color = 0;

					if (allcons[q1].color==0 && this->timer2->Enabled)
					{
						String^ s_qab = this->slist.Keys[newindex];
						s_qab = s_qab->Replace("fup.gra.akt\\", "fup.gra\\");
						s_qab = s_qab->Replace("fup.gra\\yf" , "ea."+awe_fstr+"\\bg."+bgtype_fstr+bgnum_fstr+"\\fup.sig\\qab");
						s_qab = s_qab->Replace("fup.gra\\ym" , "ea."+awe_fstr+"\\bg."+bgtype_fstr+bgnum_fstr+"\\fup.sig\\qab");
						String^ list_fstr = gcnew String(info.list);
						list_fstr = list_fstr->Trim();
						String^ kks_fstr = gcnew String(info.kks);
						kks_fstr = kks_fstr->Trim()->ToLower();
						s_qab = s_qab->Replace("#"+list_fstr+"#1", "#*");
						s_qab = s_qab->Substring(0,s_qab->IndexOf("*")+1);
						s_qab = s_qab->Replace("yf#=", "*=");
						s_qab = s_qab->Replace("ym#=", "*=");
					
						WIN32_FIND_DATAA FindFileData;
						std::memset(&FindFileData, sizeof(WIN32_FIND_DATA ),0);
						char *v6 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qab).ToPointer();
						HANDLE hf6;
						hf6=FindFirstFileA((LPCSTR)v6, (LPWIN32_FIND_DATAA) &FindFileData);
						if (hf6!=INVALID_HANDLE_VALUE) 
						{
							bool found=false;
							do	{
								String^ list_name = gcnew String(FindFileData.cFileName);
								char *f_qab_add = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qab->Replace("*="+kks_fstr+"#*",list_name)).ToPointer();
								FILE* f_qab = fopen(f_qab_add,"r");
								if (f_qab!=NULL)
								{
									char line_qab[39];
									while( !feof( f_qab ) && !found)
									{
										if (std::fread(line_qab,1,39,f_qab)<3) continue;		
										String^ line_qab_s = gcnew String(line_qab);
										String^ qab = line_qab_s->Substring(0,6)->Trim();
										String^ kks = line_qab_s->Substring(6,16)->Trim();
										String^ mark = line_qab_s->Substring(22,9)->Trim();
										String^ tel = line_qab_s->Substring(31,7)->Trim();
										if (conname_s->CompareTo(qab)==0)
										{
											//conname_s += "=" + mark; 
											conname_mark = "=" + mark;
											mark = mark->Replace(",0,",",_,");
											mark = mark->Replace("EA,","EA")->Replace("MA,","MA")->Replace("AA,","AA")->Replace("M,","M")->Replace("S,","S")->Replace("T,","T");
											mark = mark->Replace(" ","")->Replace(",0",",")->Replace(",0",",")->Replace("M0","M")->Replace("M0","M");
											mark = mark->Replace("_,","0,");
											found = true;
											String^ m_value = "";
											m_value = GetMarkerValue(awe_fstr, bgnum_fstr+","+mark, &allcons[q1].color);
											if (m_value!="") 
											{
												conname_mark += "=" + m_value;
												deep=0; ColorPartners(10, q1);
											}

										}
									};
									std::fclose(f_qab);
								}
							} while (FindNextFileA(hf6,&FindFileData)!=0 && !found);
						}
						FindClose(hf6);

					}
				
					g1->DrawString(conname_s, font1, System::Drawing::Brushes::Black, w/2+mas*allcons[q1].conname.x, h/2-mas*allcons[q1].conname.y - 1.5f*font1->SizeInPoints +1);
					if (conname_mark!="") g1->DrawString(conname_mark, font1, System::Drawing::Brushes::Black, w/2+mas*allcons[q1].conname.x, h/2 - mas*allcons[q1].conname.y +1);


					for (int k=0; k<6; k++)
					if (allcons[q1].cons[k].x!=0 && allcons[q1].cons[k].y!=0)
					{
						String^ tog2 = gcnew String((char*)allcons[q1].cons[k].text);
						g1->DrawString(tog2, font1, System::Drawing::Brushes::DarkOrange, w/2+mas*allcons[q1].cons[k].x, h/2-mas*allcons[q1].cons[k].y - 1.5f*font1->SizeInPoints +1);
					}
				}

				for (unsigned long q1=1; q1<=nPUNS; q1++)  // прорисовка соединительных точек у линий
				{
					if (allpuns[q1].num != 0 && this->debugInfoToolStripMenuItem->Checked ) g1->DrawString(Convert::ToString((unsigned char) q1), font1, System::Drawing::Brushes::Black, w/2+mas*allpuns[q1].pos.x, h/2-mas*(allpuns[q1].pos.y+1)-font1->SizeInPoints -mas+1);
					if (allpuns[q1].sq) g1->FillRectangle(Brushes::Black, w/2+mas*(allpuns[q1].pos.x-0.5f), h/2-mas*(allpuns[q1].pos.y+0.5f), mas*1.0f, mas*1.0f);
				}

				for (int unsigned long q1=1; q1<=nINPS; q1++)		// заполнение всех таблиц
				if (alltabs[q1].num!=0)
				{
					for (int k=0; k<10; k++)
					if (alltabs[q1].prop[k].x!=0 && alltabs[q1].prop[k].y!=0)
					{
						String^ tog22 = gcnew String((char*)alltabs[q1].prop[k].text);
						tog22 = tog22->Trim();
						if (k==2) g1->DrawString(tog22, font1, System::Drawing::Brushes::DarkOrange, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
						else if (this->timer2->Enabled && k==6 && tog22!="")
						{
							String^ m_value = "";
							m_value = GetMarkerValue(awe_fstr, bgnum_fstr+","+tog22->Replace(" ","")->Replace(",0",",")->Replace(",0",",")->Replace("M0","M")->Replace("M0","M"), &alltabs[q1].color);
	
							if (m_value!="") 
							{
								tog22 += "="+m_value;
								deep=0; ColorPartners(9, q1);
							}

							switch (alltabs[q1].color)
							{
								case 0:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
									break;
								case 1:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Blue, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
									break;
								case 2:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::LimeGreen, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
									break;
								case 3:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Magenta, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
									break;
								default:
									g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
									break;
							}
						}
						else if (this->timer2->Enabled && k==6 && tog22=="") // искать маркер в том же модуле
						{
							String^ s_qabt = this->slist.Keys[newindex];
							if (s_qabt->Contains("fup.gra.akt\\"))
							{
								s_qabt = s_qabt->Replace("fup.gra.akt\\", "fup.gra\\");
								s_qabt = s_qabt->Replace("."+bgtype_fstr+bgnum_fstr,"");
							}
							s_qabt = s_qabt->Replace("fup.gra\\yf" , "ea."+awe_fstr+"\\bg."+bgtype_fstr+bgnum_fstr+"\\fup.sig\\qab");
							s_qabt = s_qabt->Replace("fup.gra\\ym" , "ea."+awe_fstr+"\\bg."+bgtype_fstr+bgnum_fstr+"\\fup.sig\\qab");
							String^ list_fstrt = gcnew String((char*)info.list);
							list_fstrt = list_fstrt->Trim()->ToLower();
							String^ kks_fstrt = gcnew String((char*)info.kks);
							kks_fstrt = kks_fstrt->Trim()->ToLower();
							String^ list_part = gcnew String((char*)alltabs[q1].prop[2].text);
							list_part = list_part->Trim()->ToLower();
							String^ kks_part = gcnew String((char*)alltabs[q1].prop[1].text);
							kks_part = kks_part->Trim()->ToLower();
							if (kks_part == "") 
								continue;
							s_qabt = s_qabt->Replace(kks_fstrt, kks_part);
							s_qabt = s_qabt->Replace("#"+list_fstrt+"#1", "#*");
							s_qabt = s_qabt->Replace("yf#=", "*=");
							s_qabt = s_qabt->Replace("ym#=", "*=");
							String^ sig = gcnew String((char*)alltabs[q1].prop[3].text);
							sig = sig->Trim();
						
							WIN32_FIND_DATAA FindFileData;
							std::memset(&FindFileData, sizeof(WIN32_FIND_DATA ),0);
							char *v7 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qabt).ToPointer();
							HANDLE hf7;
							hf7=FindFirstFileA((LPCSTR)v7, (LPWIN32_FIND_DATAA) &FindFileData);
							if (hf7!=INVALID_HANDLE_VALUE) 
							{
								bool found=false;
								do	{
									String^ list_namet = gcnew String(FindFileData.cFileName);
									char *f_qab_addt = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qabt->Replace("*="+kks_part+"#*",list_namet)).ToPointer();
									FILE* f_qabt = fopen(f_qab_addt,"r");
									if (f_qabt!=NULL)
									{
										char line_qabt[39];
										while( !feof( f_qabt) && !found)
										{
											if (std::fread(line_qabt,1,39,f_qabt)<3) continue;		
											String^ line_qab_t = gcnew String(line_qabt);
											String^ qab = line_qab_t->Substring(0,6)->Trim();
											String^ kks = line_qab_t->Substring(6,16)->Trim();
											String^ mark = line_qab_t->Substring(22,9)->Trim();
											String^ tel = line_qab_t->Substring(31,7)->Trim();
										
											if (sig->CompareTo(qab)==0 && mark!="")
											{
												tog22 += "=" + mark; 
												mark = mark->Replace("EA,","EA")->Replace("MA,","MA")->Replace("AA,","AA")->Replace("M,","M")->Replace("S,","S")->Replace("T,","T");
												found = true;
												String^ m_value = "";
												m_value = GetMarkerValue(awe_fstr, bgnum_fstr+","+mark, &alltabs[q1].color);
												if (m_value!="") 
												{
													tog22 += "=" + m_value;
													deep=0; ColorPartners(9, q1);

													switch (alltabs[q1].color)
													{
														case 0:
															g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w/2+mas*alltabs[q1].prop[6].x, h/2-mas*alltabs[q1].prop[6].y -font1->SizeInPoints-mas+1);
															break;
														case 1:
															g1->DrawString(tog22, font1, System::Drawing::Brushes::Blue, w/2+mas*alltabs[q1].prop[6].x, h/2-mas*alltabs[q1].prop[6].y -font1->SizeInPoints-mas+1);
															break;
														case 2:
															g1->DrawString(tog22, font1, System::Drawing::Brushes::LimeGreen, w/2+mas*alltabs[q1].prop[6].x, h/2-mas*alltabs[q1].prop[6].y -font1->SizeInPoints-mas+1);
															break;
														case 3:
															g1->DrawString(tog22, font1, System::Drawing::Brushes::Magenta, w/2+mas*alltabs[q1].prop[6].x, h/2-mas*alltabs[q1].prop[6].y -font1->SizeInPoints-mas+1);
															break;
														default:
															g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w/2+mas*alltabs[q1].prop[6].x, h/2-mas*alltabs[q1].prop[6].y -font1->SizeInPoints-mas+1);
															break;
													}
												}
											}

											if (sig->CompareTo(qab)==0 && mark=="") // вместо маркера на листе коннектор 
																					// нужно делать вложенный цикл:(
											{

											}
										};
										std::fclose(f_qabt);
									}
								} while (FindNextFileA(hf7,&FindFileData)!=0 && !found);
							}
							FindClose(hf7);

						}
						else
						{
							g1->DrawString(tog22, font1, System::Drawing::Brushes::Black, w/2+mas*alltabs[q1].prop[k].x, h/2-mas*alltabs[q1].prop[k].y -font1->SizeInPoints-mas+1);
						}
					}
				}

							// вывод indo
				String^ tog2 = gcnew String(info.awename);
				g1->DrawString("+"+tog2, font1, System::Drawing::Brushes::Black, w/2+mas*356.2f, h/2-mas*14.2f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.kks);
				g1->DrawString("="+tog2, font1, System::Drawing::Brushes::Black, w/2+mas*356.2f, h/2-mas*18.6f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.list);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*400.0f, h/2-mas*18.6f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.type);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*348.6f, h/2-mas*18.6f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.name);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*231.0f, h/2-mas*10.6f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.awenum);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*231.0f, h/2-mas*28.0f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.date[0]);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*46.0f, h/2-mas*10.0f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.date[1]);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*46.0f, h/2-mas*14.0f -font1->SizeInPoints-mas+1);
				tog2 = gcnew String(info.date[2]);
				g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*46.0f, h/2-mas*18.0f -font1->SizeInPoints-mas+1);
				//tog2 = gcnew String(info.bpotype);
				//g1->DrawString(tog2, font1, System::Drawing::Brushes::Black, w/2+mas*231.0f, h/2-mas*68.0f -font1->SizeInPoints-mas+1);


				fg1->DrawImage(canvBitmap, PointF(0,0)); 

				if (newindex!=oldindex && newindex>=0 && this->toolStripMenuItem2->Checked) this->itea_button(sender,e);

				oldindex=newindex;


		} 
		catch (Exception^ e) 
		{
			DWORD err = GetLastError( );
			this->toolStripLabel1->Text += e->Message + Convert::ToString((int)err);
		};
	}


		 }


System::Int32 Form1::DrawElfNt(Graphics^ lg1, char* nobj, float xc, float yc)
{
	HRSRC hRes = FindResourceA(hMod, (LPCSTR)nobj, (LPCSTR)RT_RCDATA);
	if (!hRes) { return 1; }
	HGLOBAL hGlob = LoadResource(hMod, hRes);
	if (!hGlob) { return 2; }
	DWORD dwFileSize = SizeofResource(hMod, hRes);
	if (!dwFileSize) { return 3; }
	void *lpbArray = (void*)LockResource(hGlob);
	if (!lpbArray) { return 4; }

	unsigned char* in;
	in = new unsigned char[dwFileSize];
	memcpy(in, lpbArray, dwFileSize);

	unsigned char t1, t2;
	long n = 0;
	long pos = 0;
	float w = this->pictureBox1->Width + dx * 2 + mas * xc * 2;
	float h = this->pictureBox1->Height + dy * 2 - mas * yc * 2;

	pen1->Width = mas * 0.35f;
	pen1->Color = Color::Black;

	if (in != NULL)
	{
	}
}



System::Int32 Form1::DrawElf(Graphics^ lg1, char* nobj, float xc, float yc) 
		 {

			HRSRC hRes=FindResourceA(hMod,(LPCSTR)nobj,(LPCSTR)RT_RCDATA);
			if(!hRes){return 1;}
			HGLOBAL hGlob=LoadResource(hMod,hRes);
			if(!hGlob) {return 2;}
			DWORD dwFileSize=SizeofResource(hMod,hRes);
			if(!dwFileSize) {return 3;}
			void *lpbArray=(void*)LockResource(hGlob); 
			if(!lpbArray) {return 4;}

			unsigned char* in;
			in = new unsigned char[dwFileSize];
			memcpy(in,lpbArray,dwFileSize);

			 unsigned char t1, t2;
			 long n=0;
			 long pos=0;
			 float w = this->pictureBox1->Width  +  dx*2 + mas*xc*2; 
			 float h = this->pictureBox1->Height  +  dy*2  - mas*yc*2; 

			pen1->Width = mas*0.35f;
			pen1->Color = Color::Black;

			 if (in!=NULL) do
			 {
				 n++;
				 t1=in[pos]; pos++;
				 t2=in[pos]; pos++;
				 if (t1==0x25 && t2==0x10) // line type
				 {
					 t1=in[pos]; pos++;
					 t2=in[pos]; pos++;
					 pen1->DashCap = System::Drawing::Drawing2D::DashCap::Round;
					 pen1->DashOffset = 10.0f;
					 if (t1==0x01 && t2==0x00) pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Solid;
					 else 
					 {
						 pen1->DashCap = System::Drawing::Drawing2D::DashCap::Round;
						 pen1->DashStyle = System::Drawing::Drawing2D::DashStyle::Custom;
						 cli::array<Single>^temp0 = {1.0f,15.0f};
						 pen1->DashPattern = temp0;
					 }
				 }
				 if (t1==0x35 && t2==0x20) // width
				 {
				     unsigned char part[4];
					 for (int i=0; i<4; i++) {part[3-i]=in[pos]; pos++;}
					 pen1->Width = mas*DWord2Float(part);
				 }
				 else if (t1==0x14 && t2==0x80) // line
				 {
					 float point[4] = {0,0,0,0};
					 for (int j=0; j<4; j++)
					 {
						 unsigned char part[4];
						 for (int i=0; i<4; i++) {part[3-i]=in[pos]; pos++;}
						 point[j] = DWord2Float(part);
					 }
					 lg1->DrawLine(pen1, w/2+mas*point[0], h/2-mas*point[1], w/2+mas*point[2], h/2-mas*point[3]);
				 }
				 else if (t1==0xD4 && t2==0xC0) // arc
				 {
					 float arc[6] = {0,0,0,0,0,0};
					 for (int j=0; j<6; j++)
					 {
						 unsigned char part[4];
						 for (int i=0; i<4; i++) {part[3-i]=in[pos]; pos++;}
						 arc[j] = DWord2Float(part);
					 }
					 cli::array<Point>^ p3 = {Point((int)(w/2+mas*arc[0]),(int)(h/2-mas*arc[1])), Point((int)(w/2+mas*arc[2]),(int)(h/2-mas*arc[3])), Point((int)(w/2+mas*arc[4]),(int)(h/2-mas*arc[5]))};
					 lg1->DrawCurve(pen1,p3,1.0f);
				 }
				 else if (t1==0x44) // text
				 {
					 String^ s=Convert::ToString((float)(t2>>4));
					 float pointt[2] = {0,0};
					 for (int j=0; j<2; j++)
					 {
						 unsigned char part[4];
						 for (int i=0; i<4; i++) {part[3-i]=in[pos]; pos++;}
						 pointt[j] = DWord2Float(part);
					 }

					 unsigned char textlen=0;
					 t1=in[pos]; pos++;
					 t2=in[pos]; pos++;
					 if (t1==0x01 && t2==0x00)
					 {
						 char *txt;
						 {textlen=in[pos]; pos++;}
						 if (textlen>0) 
						 {
							 txt = new char[textlen+1];
							 for (int j=0; j<textlen; j++)
							 {
								  {txt[j]=in[pos]; pos++;}
							 }
							 txt[textlen]='\0';
							 String^ tog = gcnew String(txt);
							 lg1->DrawString(tog, font1, System::Drawing::Brushes::Black, w/2+mas*pointt[0], h/2-mas*pointt[1]-font1->SizeInPoints -mas+1);
							 if (textlen%2==0) {t1=in[pos]; pos++;}
						 }
					 } 
				 }

			 } while (pos<dwFileSize);

			 return 0;
		 }

System::Void Form1::pictureBox1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (actindex>0 && this->SearchResults->SelectedIndex>=0 && e->Button == System::Windows::Forms::MouseButtons::Left)
			 {
				 oldselectindex = this->SearchResults->SelectedIndex;
				 String^ oldsearch = this->FindBox->Text;
				 oldindex = -1;
				 String^ kks = this->SearchResults->SelectedItem->ToString();
				 long kksend = kks->IndexOf('#',1);
				 String^ newlist = gcnew String(allacts[actindex].list);

				 String^ newkks = "";
				 if (allacts[actindex].kks[0]!='\0') newkks = gcnew String((char*)allacts[actindex].kks);
				 else  newkks = kks->Substring(0,kksend);
				 
				 this->FindBox->Text = newkks->ToLower()+"#";

				 long newindex = this->SearchResults->FindString(newkks->ToLower()+"#"+newlist->ToLower());
				 if (newindex>=0) 
				 {
					 this->SearchResults->SelectedIndex  = newindex;
					 oldselectindex = -1;
				 }
				 else 
				 {
					 //такого ККС не нашлось, возвращаемся назад
					 this->FindBox->Text = oldsearch;
				 }
			 }

		 }
System::Void Form1::treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 this->SearchResults->Items->Clear();
			 oldindex=-1;
			 if (this->treeView1->SelectedNode->ToolTipText!="")
			 {
				 this->FindBox->Text = this->treeView1->SelectedNode->Text->Trim();
				 this->FindBox->ForeColor = Color::Black;
			 }
			 if (this->SearchResults->Items->Count > 0) this->SearchResults->SelectedIndex  = 0;
			 toolTip1->AutoPopDelay =30000; 
			 toolTip1->SetToolTip(this->treeView1, this->treeView1->SelectedNode->ToolTipText); 
			 toolTip1->IsBalloon =0;
		 }
System::Void Form1::treeView2_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		{
			this->SearchResults->Items->Clear();
			oldindex = -1;
			//if (this->treeView1->SelectedNode->ToolTipText != "")
			{
				this->FindBox->Text = this->treeView2->SelectedNode->Text->Trim();
				this->FindBox->ForeColor = Color::Black;
			}
			if (this->SearchResults->Items->Count > 0) this->SearchResults->SelectedIndex = 0;
			toolTip1->AutoPopDelay = 30000;
			toolTip1->SetToolTip(this->treeView2, this->treeView2->SelectedNode->ToolTipText);
			toolTip1->IsBalloon = 0;
		}

System::Void Form1::FindBox_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 String^ fstr = this->FindBox->Text->ToLower();
			 fstr = fstr->Replace("*","")->Replace("?","")->Replace("#","");
			 if (fstr->Length >= 3)
			 {
				 oldindex=-1;
				 fstr = fstr->Replace("0","")->Replace("1","")->Replace("2","")->Replace("3","")->Replace("4","");
				 fstr = fstr->Replace("5","")->Replace("6","")->Replace("7","")->Replace("8","")->Replace("9","");
				 if (fstr->Length < 3) {this->timer1->Interval = 1000; this->timer1->Enabled = true;}
				 else if (fstr->Length < 4) {this->timer1->Interval = 500; this->timer1->Enabled = true;}
				 else if (fstr->Length >= 4) timer1_Tick(sender,e);
			 }

		 }
System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->SearchResults->Items->Clear();

			 char* card = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi("*"+this->FindBox->Text->ToLower()+"*").ToPointer();
			 if (tabControl1->SelectedIndex == 0)
				 for (long i=0; i<this->slist.Count; i++)
				 {
					 char* val = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->slist.Values[i]).ToPointer();
					 if (WildCard(card,val)) this->SearchResults->Items->Add(this->slist.Values[i]);
				 }
			 else if (tabControl1->SelectedIndex == 1)
				 for (long i = 0; i < slint.Count; i++)
				 {
					 char* val = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(slint.Values[i]).ToPointer();
					 if (WildCard(card, val)) this->SearchResults->Items->Add(slint.Values[i]);
				 }

			 if (oldselectindex>=0 && oldselectindex < this->SearchResults->Items->Count) 
			 {
				this->SearchResults->SelectedIndex = oldselectindex;
				oldselectindex = -1;
			 }

			 this->timer1->Enabled = false;
		 }

System::Void Form1::button4_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 char *infilename = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->toolStripTextBox2->Text+"in.txt").ToPointer();
			 FILE* intask=fopen(infilename,"r");
			 if (intask!=NULL)
			 {
				 long len=0;
				 fscanf(intask,"%d",&len);
				 for (int i=0; i<len; i++)
				 {
					 this->Refresh();
					 char list[21];
					 fscanf(intask,"%s",list);
					 String^ fnd = gcnew String(list);
					 char *outfilename = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->toolStripTextBox2->Text+fnd+".txt").ToPointer();
					 FILE* out2=fopen(outfilename,"r");
					 if (out2==NULL)
					 {
						 this->FindBox->Text = fnd->ToLower();
						 this->FindBox_TextChanged(sender,e);
						 if (this->SearchResults->Items->IndexOf(this->FindBox->Text)>=0) this->SearchResults->SelectedIndex = this->SearchResults->Items->IndexOf(this->FindBox->Text);
						 //else {MessageBox::Show("Fail!");}//fail
					 }
					 else {std::fclose(out2);}
				 }
			 }
			 if (intask!=NULL) std::fclose(intask);

		 }
System::Void Form1::FindBox_Enter(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (this->FindBox->Text == "FND*ME???#01")
			 {
				 this->FindBox->Text = "";
				 this->FindBox->ForeColor = Color::Black;
			 }
		 }
System::Void Form1::FindBox_Leave(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (this->FindBox->Text == "")
			 {
				 this->FindBox->Text = "FND*ME???#01";
				 this->FindBox->ForeColor = Color::DarkGray;
			 }
		 }

System::Void Form1::sgFilesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->listBox1->Visible = !this->listBox1->Visible;
		 }
System::Void Form1::elementsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->listBox2->Visible = !this->listBox2->Visible;
		 }

System::Void Form1::Form1_SizeChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->panel1->Left= this->Width - 1192 + 952;

			this->pictureBox1->Width = this->Width - 1192 + 930;
			this->pictureBox1->Height = this->Height - 767 + 688;

			this->listBox2->Top = this->Height - 767 + 634;
			this->listBox1->Top = this->Height - 767 + 400;

			 //MessageBox::Show("SizeChanged");
			 if (this->pictureBox1->Width<=0) this->pictureBox1->Width=1;
			 if (this->pictureBox1->Height<=0) this->pictureBox1->Height=1;
			 if (tabControl1->SelectedIndex == 0)
			 {
				 if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 420.0f / 297.0f) mas = this->pictureBox1->Width / 420.0f;
				 else mas = this->pictureBox1->Height / 297.0f;
				 dx = -210.0f*mas; dy = 148.5f*mas;
				 font1 = gcnew System::Drawing::Font("MS Sans Serif", 2.0f*mas);
			 }
			 else if (tabControl1->SelectedIndex == 1)
			 {
				 if ((float)this->pictureBox1->Width / this->pictureBox1->Height < 1112.0f / 782.0f) mas = this->pictureBox1->Width / 1112.0f;
				 else mas = this->pictureBox1->Height / 782.0f;
				 dx = -556.0f*mas; dy = 391.0f*mas;
				 font1 = gcnew System::Drawing::Font("MS Sans Serif", 6.0f*mas);
			 }
			 
			 this->label1->Text = Convert::ToString(dx);
			 this->label2->Text = Convert::ToString(dy);
			 this->label3->Text = Convert::ToString(mas);

		 	 canvBitmap = gcnew Bitmap(this->pictureBox1->Width, this->pictureBox1->Height); 
			 fg1 = this->pictureBox1->CreateGraphics();
			 g1 = Graphics::FromImage(canvBitmap);


		 }
System::Void Form1::DebuggerButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if (!this->PlayButton->Enabled && this->StopButton->Enabled)
			{
				 try
				 {
					 if (this->DebuggerButton->BackColor == Color::SkyBlue) 
					 {
						 debugger->WindowState = FormWindowState::Minimized;
						 this->DebuggerButton->BackColor = Color::WhiteSmoke;
					 }
					 else 
					 {
						 debugger->WindowState = FormWindowState::Normal;
						 this->DebuggerButton->BackColor = Color::SkyBlue;
					 }
				 }
				 catch (Exception^ e) 
				 {
					DWORD err = GetLastError( );
					this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
					this->DebuggerButton->BackColor = Color::WhiteSmoke;
				 }
			}
		 }

System::Void Form1::itea_button(System::Object^  sender, System::EventArgs^  e) 
		 { 
			 //вывод всех найденных на листе элементов в текстовый файл
			 if (newindex>0)
			 {
				 char *outfilename = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(this->toolStripTextBox2->Text+"\\"+"ita_"+this->slist.Values[newindex]+".txt").ToPointer();
				 FILE* itea=fopen(outfilename,"w");
				 if (itea!=NULL)
				 {
						fprintf(itea,"nPUNS = %d\n", nPUNS);
						fprintf(itea,"nLINS = %d\n", nLINS);
						fprintf(itea,"nOBJS = %d\n", nOBJS);
						fprintf(itea,"nINPS = %d\n", nINPS);
						fprintf(itea,"nCONS = %d\n", nCONS);

					for (unsigned long q1=1; q1<=nOBJS; q1++)		// вывод информации о всех ubi
					if (allobjs[q1].num!=0 && allobjs[q1].type[0]!='\n' /*&& allobjs[q1].type < this->listBox1->Items->Count*/)
					{
						fprintf(itea,"\n*** new ubi ***\n");
						fprintf(itea,"num = %d\n", allobjs[q1].num);
						fprintf(itea,"type = %s\n", allobjs[q1].type);
						fprintf(itea,"pos = %.2f %.2f\n", allobjs[q1].pos.x, allobjs[q1].pos.y);
						for (int q2=0; q2<16; q2++)
						if (allobjs[q1].t[q2].text[0]!='\0')
						{
							fprintf(itea,"par[%d] = %s, parpos = %.2f %.2f, vis = %d\n", q2, allobjs[q1].t[q2].text, allobjs[q1].t[q2].x, allobjs[q1].t[q2].y, allobjs[q1].t[q2].vis);
						}
					}
					for (unsigned long q1=1; q1<=nINPS; q1++)		// вывод информации о всех tab
					if (alltabs[q1].num!=0)
					{
						fprintf(itea,"\n*** new tab ***\n");
						fprintf(itea,"num = %d\n", alltabs[q1].num);
						fprintf(itea,"pos = %.2f %.2f\n", alltabs[q1].pos.x, alltabs[q1].pos.y);
						for (int q2=0; q2<10; q2++)
						if (alltabs[q1].prop[q2].text[0]!='\0')
						{
							fprintf(itea,"par[%d] = %s, parpos = %.2f %.2f, vis = %d\n", q2, alltabs[q1].prop[q2].text, alltabs[q1].prop[q2].x, alltabs[q1].prop[q2].y, alltabs[q1].prop[q2].vis);
						}
					}
					for (unsigned long q1=1; q1<=nLINS; q1++)		// вывод информации о всех lin
					if (alllins[q1].num!=0)
					{
						fprintf(itea,"\n*** new lin ***\n");
						fprintf(itea,"num = %d\n", q1);
						fprintf(itea,"addr = 0x%04x\n", linsadd[q1]);
						fprintf(itea,"dashtype = %d\n", alllins[q1].type);
						fprintf(itea,"srcobjnum = %d\n", alllins[q1].nobj[0]);
						fprintf(itea,"srcobjtyp = %d\n", alllins[q1].tobj[0]);
						fprintf(itea,"dstobjnum = %d\n", alllins[q1].nobj[1]);
						fprintf(itea,"dstobjtyp = %d\n", alllins[q1].tobj[1]);
						fprintf(itea,"vlf = %.2f %.2f %.2f %.2f\n", alllins[q1].x1, alllins[q1].y1, alllins[q1].x2, alllins[q1].y2);
					}
					for (unsigned long q1=1; q1<=nINPS; q1++)		// вывод информации о всех inp
					if (allinps[q1].num!=0)
					{
						fprintf(itea,"\n*** new inp ***\n");
						fprintf(itea,"num = %d\n", allinps[q1].num);
						for (int q2=0; q2<5; q2++)
						if (allinps[q1].pos[q2].x!=0.0f || allinps[q1].pos[q2].y!=0.0f)
						{
							fprintf(itea,"pos[%d] = %.2f %.2f\n", q2, allinps[q1].pos[q2].x, allinps[q1].pos[q2].y);
						}
						fprintf(itea,"name = %s\n", allinps[q1].name);
						fprintf(itea,"negin = %d\n", allinps[q1].negin);
						fprintf(itea,"negout = %d\n", allinps[q1].negout);
						fprintf(itea,"parentobj = %d\n", allinps[q1].parent);
						for (int q2=0; q2<8; q2++)
						if (allinps[q1].t[q2].text[0]!='\0')
						{
							fprintf(itea,"par[%d] = %s, parpos = %.2f %.2f, vis = %d\n", q2, allinps[q1].t[q2].text, allinps[q1].t[q2].x, allinps[q1].t[q2].y, allinps[q1].t[q2].vis);
						}
						fprintf(itea,"linelink = %d\n", allinps[q1].linelink);
						
					}
					for (unsigned long q1=1; q1<=nPUNS; q1++)		// вывод информации о всех pun
					if (allpuns[q1].num!=0)
					{
						fprintf(itea,"\n*** new pun ***\n");
						fprintf(itea,"num = %d\n", q1);
						fprintf(itea,"addr = 0x%04x\n", punsadd[q1]);
						fprintf(itea,"sq = %d\n", allpuns[q1].sq);
						fprintf(itea,"pos = %.2f %.2f\n", allpuns[q1].pos.x, allpuns[q1].pos.y);
						for (int q2=0; q2<4; q2++) 
						if (allpuns[q1].nlink[q2]!=0) fprintf(itea,"tlink[%d] = %d, nlink[%d] = %d\n", q2, allpuns[q1].tlink[q2], q2, allpuns[q1].nlink[q2]);
					}

					for (unsigned long q1=1; q1<=nCONS; q1++)		// вывод информации о всех con
					if (allcons[q1].num!=0)
					{
						fprintf(itea,"\n*** new con ***\n");
						fprintf(itea,"num = %d\n", allcons[q1].num);
						fprintf(itea,"pos = %.2f %.2f\n", allcons[q1].pos.x, allcons[q1].pos.y);
						fprintf(itea,"conname = %s, connamepos = %.2f %.2f, vis = %d\n", allcons[q1].conname.text, allcons[q1].conname.x, allcons[q1].conname.y, allcons[q1].conname.vis);
						if (allcons[q1].slashtype[0]!='\0')
						fprintf(itea,"slashtype = %s\n", allcons[q1].slashtype);
						for (int q2=0; q2<10; q2++)
						if (allcons[q1].cons[q2].text[0]!='\0')
						{
							fprintf(itea,"con[%d] = %s, conpos = %.2f %.2f, vis = %d\n", q2, allcons[q1].cons[q2].text, allcons[q1].cons[q2].x, allcons[q1].cons[q2].y, allcons[q1].cons[q2].vis);
						}
						fprintf(itea,"punlink = %d\n", allcons[q1].punlink);
					}
					std::fclose(itea);
				 }
			 }

		 }

System::Void Form1::UndoButton_DropDownItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) 
		 {
			 undoredo = true;
 			 this->FindBox->Text = e->ClickedItem->ToString();
			 if (this->SearchResults->Items->Count>=1) this->SearchResults->SelectedIndex = 0;
			 if (histpos>=0) this->UndoButton->DropDownItems[histpos]->BackColor = SystemColors::Control;
			 histpos=this->UndoButton->DropDownItems->IndexOf(e->ClickedItem);
			 this->UndoButton->DropDownItems[histpos]->BackColor = Color::DarkGray;
		 }
System::Void Form1::UndoButton_ButtonClick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 undoredo = true;
			 if (histpos>0) 
			 { 
				 if (histpos>=0) this->UndoButton->DropDownItems[histpos]->BackColor = SystemColors::Control;
				 histpos--;
 				this->FindBox->Text = this->UndoButton->DropDownItems[histpos]->Text;
				if (this->SearchResults->Items->Count>=1) this->SearchResults->SelectedIndex = 0;
				//this->UndoButton->DropDownItems[histpos]->BackColor = Color::LightGray;
			 }
		 }
/*System::Void Form1::NobiButton_ButtonClick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 undoredo = true;
			 if (histpos>0) histpos++;
 			 //this->FindBox->Text = this->NobiButton->DropDownItems[histpos]->Text;
			 //if (this->SearchResults->Items->Count>=1) this->SearchResults->SelectedIndex = 0;
		 }*/
System::Void Form1::RedoButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 undoredo = true;
			 if (histpos<this->UndoButton->DropDownItems->Count-1) 
			 { 
				 if (histpos>=0) this->UndoButton->DropDownItems[histpos]->BackColor = SystemColors::Control;
				 histpos++;
 				this->FindBox->Text = this->UndoButton->DropDownItems[histpos]->Text;
				if (this->SearchResults->Items->Count>=1) this->SearchResults->SelectedIndex = 0;
				//this->UndoButton->DropDownItems[histpos]->BackColor = Color::LightGray;
			 }
		 }

System::Void Form1::pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 	if (this->SearchResults->SelectedIndex >=0 && !this->FindBox->Focused && !gscroll) this->SearchResults_SelectedIndexChanged(sender,e);
		 }

System::Void Form1::toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			myforecolor = Color::White;
			if (this->SearchResults->SelectedIndex >=0) this->SearchResults_SelectedIndexChanged(sender,e);
			canvBitmap->Save(this->toolStripTextBox2->Text + this->SearchResults->SelectedItem + ".jpg");
			myforecolor = Color::WhiteSmoke;
		 }

System::Void Form1::PlayButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->PlayButton->Enabled = false;
			 int n_awe = 0;

			 if (tnc == nullptr)
			 {
				 tnc = tw->Nodes;
				 tnc->Clear();

				 if (this->tabControl1->SelectedTab->TabIndex == 0)
				 {
					 for (int na = 0; na < this->treeView1->Nodes->Count; na++)
					 {
						 tnc->Add(this->treeView1->Nodes[na]->Text);
						 for (int nm = 0; nm < this->treeView1->Nodes[na]->Nodes->Count; nm++)
							 if (this->treeView1->Nodes[na]->Nodes[nm]->ToolTipText != "")
							 {
								 tnc[tnc->Count - 1]->Nodes->Add(this->treeView1->Nodes[na]->Nodes[nm]->Text);
								 tnc[tnc->Count - 1]->Nodes[tnc[tnc->Count - 1]->Nodes->Count - 1]->ToolTipText = this->treeView1->Nodes[na]->Nodes[nm]->ToolTipText;
							 }
					 }
				 }
				 else if (this->tabControl1->SelectedTab->TabIndex == 1)
				 {
					 for (int nptk = 0; nptk < this->treeView2->Nodes->Count; nptk++)
						 for (int na = 0; na < this->treeView2->Nodes[nptk]->Nodes->Count; na++)
						 {
							 tnc->Add(this->treeView2->Nodes[nptk]->Nodes[na]->Text);
							 for (int nm = 0; nm < this->treeView2->Nodes[nptk]->Nodes[na]->Nodes->Count; nm++)
								 if (this->treeView2->Nodes[nptk]->Nodes[na]->Nodes[nm]->ToolTipText != "")
								 {
									 tnc[tnc->Count - 1]->Nodes->Add(this->treeView2->Nodes[nptk]->Nodes[na]->Nodes[nm]->Text);
									 tnc[tnc->Count - 1]->Nodes[tnc[tnc->Count - 1]->Nodes->Count - 1]->ToolTipText = this->treeView2->Nodes[nptk]->Nodes[na]->Nodes[nm]->ToolTipText;
								 }
						 }
				 }
			 }

			 n_awe = tnc->Count;
			 if (n_awe < 0) n_awe=0;
			 if (n_awe > 300) n_awe = 300;
			 //nobi = gcnew List<Form^>();

			 if (!this->timer2->Enabled && !this->StopButton->Enabled ) // первый запуск
			 {
				 tabrel_a = gcnew cli::array<List<String^>^,1>(n_awe);// Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabrel_b = gcnew cli::array<List<String^>^,1>(n_awe);// Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabrel_t = gcnew cli::array<List<String^>^,1>(n_awe);// Таблица соответствия таймеров стойки. Позиция в списке == номер таймера в коде.
				 tabrel_a_emt2raek = gcnew cli::array<List<String^>^, 1>(1);// Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabrel_a_raek2emt = gcnew cli::array<List<String^>^, 1>(1);// Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabrel_b_emt2raek = gcnew cli::array<List<String^>^, 1>(1);// Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabrel_b_raek2emt = gcnew cli::array<List<String^>^, 1>(1);// Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
				 tab_a = new float*[n_awe];// Массив аналоговых значений стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_a_emt2raek = new float[EMT_TO_RAEK_ANALOG];// Массив аналоговых значений всех стоек, передаваемых в РАЭК. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_a_raek2emt = new float[EMT_FROM_RAEK_ANALOG];// Массив аналоговых значений всех стоек, принимаемых от РАЭК. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_bba = new int*[n_awe];// Массив битов-сопроводителей аналогов стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_t = new float*[n_awe];// Массив таймеров стойки. Меняется в процессе обработки кода. Позиция в списке == номер таймера в коде.
				 tab_b = new int*[n_awe];// Массив дискретных значений стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_b_emt2raek = new int[EMT_TO_RAEK_DISCRETE];// Массив дискретных значений всех стоек, передаваемых в РАЭК. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_b_raek2emt = new int[EMT_FROM_RAEK_DISCRETE];// Массив дискретных значений всех стоек, принимаемых от РАЭК. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
				 tab_c = gcnew cli::array<List<int>^,1>(n_awe);// Массив упрощенных команд всех модулей стойки. Результат разложения кода.
				 tab_m = gcnew cli::array<List<int>^,1>(n_awe);// Массив адресов ячеек памяти всех модулей стойки. Результат разложения кода.
				 tabarh_a = gcnew cli::array<List<String^>^,1>(n_awe);// Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
				 tabarh_b = gcnew cli::array<List<String^>^,1>(n_awe);// Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
				 tab_pb = gcnew cli::array<List<int>^,1>(n_awe);// Таблица соответствия описаний аналоговых маркеров
				 tab_pa = gcnew cli::array<List<int>^,1>(n_awe);// Таблица соответствия описаний дискретных маркеров

				 emulated = gcnew ArtMoney::AWE();
				 emulated->treeView1->Nodes->Clear();	
				 emulated->Show();				 
				 
				 debugger = gcnew ArtMoney::Form2();
				 debugger->Show();
				 debugger->WindowState = FormWindowState::Minimized;


				 
		// группировка шкафов по сегментам
				 String^ awe_name = "";
				 String^ segment_name = "";
				 for (int i_awe=0; i_awe<n_awe; i_awe++)
				 {
					 awe_name = tnc[i_awe]->Text;
					 int gap = awe_name->IndexOf("(");
					 if (gap>=3)
					 {
						 if (awe_name->Substring(0,gap-2) != segment_name )
						 {
							if (segment_name!="" && i_awe>0 && emulated->treeView1->Nodes->Count > 0 &&  emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Nodes->Count > 1)
							{
								emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Text += "-" + tnc[i_awe-1]->Text->Substring(tnc[i_awe-1]->Text->IndexOf("(")-2,2);
							}
							segment_name = awe_name->Substring(0,gap-2);
							emulated->treeView1->Nodes->Add(awe_name->Substring(0,gap));
							emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Nodes->Add(awe_name);
						 }
						 else if (emulated->treeView1->Nodes->Count > 0)
						 {
							 emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Nodes->Add(awe_name);
						 }

						 if (i_awe == n_awe-1 && segment_name!="" && emulated->treeView1->Nodes->Count > 0 &&  emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Nodes->Count > 1)
						 {
								emulated->treeView1->Nodes[emulated->treeView1->Nodes->Count-1]->Text += "-" + tnc[i_awe]->Text->Substring(tnc[i_awe]->Text->IndexOf("(")-2,2);
						 }
					 }
				 }

				 this->timer2->Enabled = true;
				 this->DebuggerButton->Enabled = true;
				 this->EmulatorButton->Enabled = true;
				 this->EmulatorButton->BackColor = Color::SkyBlue;
				 archive = fopen("archive.txt", "a");
			 }
			 else if (this->timer2->Enabled && !emulated->is_ready) // поменялся выбор шкафов
			 {
				 int count_est=0;
				 this->timer2->Enabled = false;
				 for (int i_awe=0; i_awe<n_awe; i_awe++)
				 if (emulated->is_changed[i_awe])
				 try{
					if (tab_c[i_awe]->Count > 1) emulated->is_changed[i_awe]=false;
				 }
				 catch (...) //раскладываем все коды модулей на команды и ячейки памяти
				 {
					 this->Refresh();
					 tabrel_a[i_awe]=gcnew List<String^>(); // Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
					 tabrel_b[i_awe]=gcnew List<String^>(); // Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
					 tabrel_t[i_awe]=gcnew List<String^>(); // Таблица соответствия таймеров стойки. Позиция в списке == номер таймера в коде.
					 tab_c[i_awe]=gcnew List<int>(); // Массив упрощенных команд всех модулей стойки. Результат разложения кода.
					 tab_m[i_awe]=gcnew List<int>(); // Массив адресов ячеек памяти всех модулей стойки. Результат разложения кода.
					 tabarh_a[i_awe]=gcnew List<String^>(); // Таблица соответствия аналоговых маркеров стойки. Позиция в списке == номер маркера в коде.
					 tabarh_b[i_awe]=gcnew List<String^>(); // Таблица соответствия дискретных маркеров стойки. Позиция в списке == номер маркера в коде.
					 tab_pb[i_awe]=gcnew List<int>();// Таблица соответствия описаний аналоговых маркеров
					 tab_pa[i_awe]=gcnew List<int>();// Таблица соответствия описаний дискретных маркеров

					 // обработка кода EAS
					 int i_eas=-1;
					 String^ eas_code="";
					 String^ t_code="";

					/* if (this->tabControl1->SelectedTab->TabIndex == 0)
					 {
						 this->treeView1->Nodes->IndexOf(tnc[i_awe]->Text);
					 }
					 else if (this->tabControl1->SelectedTab->TabIndex == 1)
					 {

					 }*/

					 //MessageBox::Show("tnc[i_awe]->Nodes->Count = "+ Convert::ToString(tnc[i_awe]->Text));

					 for (int i_bg = 0; i_bg < tnc[i_awe]->Nodes->Count; i_bg++)
					 if (tnc[i_awe]->Nodes[i_bg]->Text->Contains("eas"))
					 {
						 i_eas = i_bg; // в списке шкафа нашли модуль eas
						 i_bg = tnc[i_awe]->Nodes->Count;
						 continue;
					 }
					 if (i_eas>=0)
					 {
						 eas_code = tnc[i_awe]->Nodes[i_eas]->ToolTipText;
						 eas_code = eas_code->Replace(",A,T,", ",E,S,");
							
						 int res=0;
						 int regim=0;
						 String^ tel="";
						 String^ ch="";
						 String^ ekb="";
						 String^ kk="";
						 String^ mod="";
						 String^ mark="";
						 do
						 {
							 res = this->step_splitter(eas_code, splitlist);
							 if (res>0)
							 {
								if (splitlist->Count == 2 && splitlist[0]=="EKB") // номер шкафа в пределах сегмента
								{
									regim=0;
									String^ zero = "000";
									ekb = "EK" + zero->Substring(0,3-splitlist[1]->Length) + splitlist[1];
									regim=1;
								}
								if (splitlist->Count == 3 && splitlist[0]=="KK") // найдено описание внешней телеграммы
								{
									String^ zero = "000";
									kk = "KK" + zero->Substring(0,3-splitlist[1]->Length) + splitlist[1];
									tel += splitlist[2];
								}
								if (splitlist->Count == 2 && splitlist[0]->Contains("TEXT")) // начало инициализации компонентов шкафа
								{
									// пока непонятные текстовые переменные в старых бэкапах их нету.
									regim=1;
								}
								if (splitlist->Count == 3 && splitlist[0]=="ZYK") // начало циклической обработки
								{
									regim=0;
								}

								if (splitlist->Count == 4 && (splitlist[0]=="AWS" || splitlist[0]=="BWS" || splitlist[0]=="AWE" || splitlist[0]=="BWE"))
								{
									mod = splitlist[2]->ToLower() + "_" + splitlist[1];
									ch = splitlist[3];
								}

								if (splitlist->Count == 1 && splitlist[0]=="TAW") // начало аналоговой пересылки между модулями
								{
									regim=7;
								}
								else if (splitlist->Count == 3 && splitlist[0]=="AW")
								{
									mod = splitlist[2]->ToLower() + "_" + splitlist[1];
								}
								else if (splitlist->Count == 4 && splitlist[0]=="QAA")
								{
									mark = splitlist[1]+","+splitlist[2]+","+splitlist[3];
									// хорошо бы добавить бит сопровождения
									if (regim == 7) tnc[i_awe]->ToolTipText += "LAD," + mod + "," + mark + ";\n";
									else if (regim == 3) tnc[i_awe]->ToolTipText += "LAD+," + mod + "," + mark + ";\nZUW+," + ekb + "," + tel + "," + ch + ";\n";
								}
								else if (splitlist->Count == 4 && splitlist[0]=="ZAA")
								{
									mark = splitlist[1]+","+splitlist[2]+","+splitlist[3];
									if (regim == 7) tnc[i_awe]->ToolTipText += "ZUW," + mod + "," + mark + ";\n";
									else if (regim == 5) tnc[i_awe]->ToolTipText += "LAD," + kk + "," + tel + "," + ch + ";\nZUW," + mod + "," + mark + ";\n";
								}

								if (splitlist->Count == 1 && splitlist[0]=="TBW") // начало дискретной пересылки между модулями
								{
									regim=6;
								}
								else if (splitlist->Count == 3 && splitlist[0]=="BW")
								{
									mod = splitlist[2]->ToLower() + "_" + splitlist[1];
								}
								else if (splitlist->Count == 4 && splitlist[0]=="QAB")
								{
									mark = splitlist[1]+","+splitlist[2]+","+splitlist[3];
									if (regim == 6) tnc[i_awe]->ToolTipText += "LADB," + mod + "," + mark + ";\n";
									else if (regim == 2) tnc[i_awe]->ToolTipText += "LADB+," + mod + "," + mark + ";\n:+," + ekb + "," + tel + "," + ch + ";\n";
								}
								else if (splitlist->Count == 4 && splitlist[0]=="ZAB")
								{
									mark = splitlist[1]+","+splitlist[2]+","+splitlist[3];
									if (regim == 6) tnc[i_awe]->ToolTipText += ":," + mod + "," + mark + ";\n";
									else if (regim == 4) tnc[i_awe]->ToolTipText += "LADB," + kk + "," + tel + "," + ch + ";\n:," + mod + "," + mark + ";\n";
								}

								if (splitlist->Count == 3 && (splitlist[0]=="MKS" || splitlist[0]=="BES")) // найдено описание внутренней пересылки
								{
									regim=2;
									tel=splitlist[0]+splitlist[1];
								}
								else if (splitlist->Count == 3 && (splitlist[0]=="AKS" || splitlist[0]=="AES")) // найдено описание внешней телеграммы
								{
									regim=3;
									tel=splitlist[0]+splitlist[1];
								}
								else if (splitlist->Count == 2 && (splitlist[0]=="MKE" || splitlist[0]=="BEE")) // найдено описание внутренней пересылки
								{
									regim=4;
									//tel=splitlist[0]+splitlist[1];
									tel=splitlist[0]->Substring(0,2)+"S";
								}
								else if (splitlist->Count == 2 && (splitlist[0]=="AKE" || splitlist[0]=="AEE")) // найдено описание внешней телеграммы
								{
									regim=5;
									//tel=splitlist[0]+splitlist[1];
									tel=splitlist[0]->Substring(0,2)+"S";
								}

								if (splitlist->Count == 1 && splitlist[0]=="ENDE") // найдено описание внешней телеграммы
								{
									regim=0;
								}
								switch(regim)
								{
									case 0:
										if (res+2<eas_code->Length) tnc[i_awe]->ToolTipText += eas_code->Substring(0,res+2);
									break;

									case 1:
										if (splitlist->Count == 3) // найдена строка описания модуля
										{
											if (splitlist[1]!="0" && splitlist[2]=="0") //splitlist[0] -mod_type; splitlist[1] -mod_1; splitlist[2] -mod_2
											{
												tnc[i_awe]->ToolTipText += splitlist[0]->ToLower() + splitlist[1] + ";\n";
											}
											else if (splitlist[1]=="0" && splitlist[2]!="0") 
											{
												tnc[i_awe]->ToolTipText += splitlist[0]->ToLower() + splitlist[2] + ";\n";
											}
											else if (splitlist[1]!="0" && splitlist[2]!="0") 
											{
												tnc[i_awe]->ToolTipText += splitlist[0]->ToLower() + splitlist[1] + " " + splitlist[0]->ToLower() + splitlist[2] +";\n";
											}
										}
										else if (splitlist->Count == 4) // найдена строка описания интерфейсного блочка
										tnc[i_awe]->ToolTipText += splitlist[0] + " " + splitlist[1] + " " + splitlist[2] + " " + splitlist[3] +";\n";
									break;


									default:
									break;
								}

								if (res+2<eas_code->Length) eas_code = eas_code->Substring(res+2);
								else eas_code="";
							 }
						 } while (res>0);
					 }

					 // назначение упрощенных адресов маркерам в коде eas
					 t_code = tnc[i_awe]->ToolTipText;
					 tabrel_a[i_awe]->Clear();
					 tabrel_b[i_awe]->Clear();
					 tabrel_t[i_awe]->Clear();
					 tab_c[i_awe]->Clear();
					 tab_m[i_awe]->Clear();
					 tabrel_a[i_awe]->Add(tnc[i_awe]->Text);
					 tabrel_b[i_awe]->Add(tnc[i_awe]->Text);
					 tabrel_t[i_awe]->Add(tnc[i_awe]->Text);

					 tabarh_a[i_awe]->Clear();
					 tabarh_b[i_awe]->Clear();
					 tabarh_a[i_awe]->Add(tnc[i_awe]->Text);
					 tabarh_b[i_awe]->Add(tnc[i_awe]->Text);
					 tab_pa[i_awe]->Clear();
					 tab_pb[i_awe]->Clear();
					 tab_pa[i_awe]->Add(0);
					 tab_pb[i_awe]->Add(0);
					 
					 bool finished = true;
					 String^ marker = "";
					 String^ marker2 = "";
					 String^ rel = "";
					 String^ s_qab1 = "";
					 String^ bg_full_num = "";
					 String^ bg_short_num = "";

					 cli::array <String^>^ eas_am_list = {"LAD+","LADB+","ZUW+",":+" ,"LAD","ZUW","LADB",":" };
					 cli::array <bool>^ eas_type_list =  {true  ,false  ,true  ,false,true ,true ,false ,false};
					 cli::array <String^>^ eas_com_list = {"@13","@10","@14","@4","@13","@14","@10","@4" };
					 for (int n_am=0; n_am<eas_am_list->Length; n_am++)
					 {
						 do
						 {
							 finished = true;
							 int m_pos = t_code->IndexOf(eas_am_list[n_am]+",");
							 int m_pos2 = t_code->IndexOf(";\n",m_pos+1);
							 if (m_pos > 0)
							 {
								 marker = t_code->Substring(m_pos+eas_am_list[n_am]->Length+1, m_pos2-m_pos-eas_am_list[n_am]->Length-1);
								 int isnew = marker->IndexOf(",");
								 if (isnew>0) // нужно назначать новый маркер (или искать его в таблице)
								 {
									 String^ tstr = marker->Substring(0,isnew);
									 bg_full_num = tstr->Replace("_","");
									 bg_short_num = tstr->Substring( tstr->IndexOf("_")+1 );
									 marker2 = marker->Substring(isnew+1);

									 if (eas_type_list[n_am]) rel = Convert::ToString(tabrel_a[i_awe]->Count);
									 else rel = Convert::ToString(tabrel_b[i_awe]->Count);

									 t_code = t_code->Substring(0, m_pos)->Replace(marker+";", "#"+rel+";") + eas_com_list[n_am] + "," + "#" + rel + t_code->Substring(m_pos2)->Replace(marker+";", "#"+rel+";");
									 
									 bool found = false;

									 if (this->tabControl1->SelectedTab->TabIndex == 0)
									 {
										 if (eas_am_list[n_am] == "LAD+" || eas_am_list[n_am] == "LADB+") // поиск расширений сигналов соотв. маркеров для ведения архива 
										 {
											 s_qab1 = this->comboBox1->Text + "sicherung\\a." + this->treeView1->Nodes[i_awe]->Text->Replace("(", "\\ea.")->Replace(")", "\\bg." + bg_full_num + "\\fup.sig\\qab\\*");
											 WIN32_FIND_DATAA FindFileData;
											 std::memset(&FindFileData, sizeof(WIN32_FIND_DATA), 0);
											 char *v7 = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qab1).ToPointer();
											 HANDLE hf7;
											 hf7 = FindFirstFileA((LPCSTR)v7, (LPWIN32_FIND_DATAA)&FindFileData);
											 if (hf7 != INVALID_HANDLE_VALUE)
											 {
												 do {
													 String^ f_name = gcnew String(FindFileData.cFileName);
													 char *f_qab_add = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s_qab1->Replace("*", f_name)).ToPointer();
													 FILE* f_qab = fopen(f_qab_add, "r");
													 String^ good_qab = "";
													 if (f_qab != NULL)
													 {
														 char line_qab[39];
														 while (!feof(f_qab) && !found)
														 {
															 if (std::fread(line_qab, 1, 39, f_qab) < 3) continue;
															 String^ line_qab_s = gcnew String(line_qab);
															 String^ qab = line_qab_s->Substring(0, 6)->Trim();
															 String^ part = line_qab_s->Substring(6, 16)->Trim();
															 String^ ma = line_qab_s->Substring(22, 9)->Trim();
															 String^ te = line_qab_s->Substring(31, 7)->Trim();
															 if (qab != "") good_qab = qab;
															 if (te != "" && ma->CompareTo(marker2) == 0)
															 {
																 found = true;
																 if (eas_type_list[n_am])
																 {
																	 tab_pa[i_awe]->Add(tabarh_a[i_awe]->Count);
																	 tabarh_a[i_awe]->Add(f_name->Substring(f_name->IndexOf("#") + 2, f_name->IndexOf("#", 4) - f_name->IndexOf("#") - 2)->ToUpper() + "_" + qab);
																 }
																 else
																 {
																	 tab_pb[i_awe]->Add(tabarh_b[i_awe]->Count);
																	 tabarh_b[i_awe]->Add(f_name->Substring(f_name->IndexOf("#") + 2, f_name->IndexOf("#", 4) - f_name->IndexOf("#") - 2)->ToUpper() + "_" + qab);
																 }
															 }
														 };
														 std::fclose(f_qab);
													 }
												 } while (FindNextFileA(hf7, &FindFileData) != 0 && !found);
											 }
											 FindClose(hf7);
										 }
										 else if (eas_am_list[n_am] == "ZUW+" || eas_am_list[n_am] == ":+")
										 {
											 found = true;
											 if (eas_type_list[n_am]) tab_pa[i_awe]->Add(-1);
											 else tab_pb[i_awe]->Add(-1);
										 }
									 }

									 marker2 = bg_short_num + "," + marker2->Replace("M,","M")->Replace("E,","E")->Replace("S,","S")->Replace("A,","A")->Replace("T,","T");
									 if (eas_type_list[n_am]) 
									 {
										 tabrel_a[i_awe]->Add(marker2);
										 if (!found) tab_pa[i_awe]->Add(0); 
									 }
									 else 
									 {
										 tabrel_b[i_awe]->Add(marker2);
										 if (!found) tab_pb[i_awe]->Add(0); 
									 }							
								 }
								 else 
								 {
									t_code = t_code->Substring(0, m_pos) + eas_com_list[n_am] + "," + marker + t_code->Substring(m_pos2);
								 }

								 finished = false;
							 }
						 } while (!finished);
					 }
					
					 //tnc[i_awe]->ToolTipText = t_code;

					 // назначение упрощенных адресов маркерам в кодах модулей и
					 // назначение упрощенных кодов команд в кодах модулей и в коде eas

					 for (int i_bg=-i_eas; i_bg<tnc[i_awe]->Nodes->Count; i_bg++)
					 if (i_bg==-i_eas || (i_bg>=0 && i_bg!=i_eas && tnc[i_awe]->ToolTipText->Contains(tnc[i_awe]->Nodes[i_bg]->Text)))
					 {
						 String^ bg_short_num = "";
						 int sp_beg=-1, sp_end=-1;
						 
						 if (i_bg!=-i_eas) 
						 {
							 t_code = tnc[i_awe]->Nodes[i_bg]->ToolTipText;
							 sp_beg = tnc[i_awe]->Nodes[i_bg]->ToolTipText->IndexOf("\nSP,");
							 sp_end = tnc[i_awe]->Nodes[i_bg]->ToolTipText->IndexOf(";",sp_beg+1);
							 if (sp_beg>0 && sp_end>0 && sp_end>sp_beg) bg_short_num = tnc[i_awe]->Nodes[i_bg]->ToolTipText->Substring(sp_beg+4, sp_end-sp_beg-4);
						 }
						 else 
						 {
							 //t_code = tnc[i_awe]->ToolTipText;
						 }

						 bool finished;
						 // поиск и замена интерфейсных блоков списком маркеров
						 t_code = t_code->Replace(",MK;",",M;")->Replace(",VK;",",V;")->Replace(",SK;",",S;")->Replace("\nESB,","\nESG,");
						 cli::array <String^>^ blok_list =     {"REK1","REK2","BA1","BA2","VW1","VW2","ESG,1,M","ESG,2,M","ESG,3,M","ESG,4,M","ESG,1,V","ESG,2,V","ESG,3,V","ESG,4,V","ESG,5,V","RES1","RES2","ESG,1,S","ESG,2,S","ESG,3,S","ESG,4,S","ITE,1","ITE,2","ITE,3","ITE,4","ITE,5","ITE,6","ITE,7","ITE,8","IVL,1","IVL,2","IVL,3","IVL,4","IVL,5","IVL,6","IVL,7","IVL,8","RLG1","RLG2","IBR,1","IBR,2","IBR,3","IBR,4","IBR,5","IBR,6","IBR,7","IBR,8","SWB1","SWB2"};
                         cli::array <String^>^ blok_com_list = {  "38", "38" ,"39" ,"39" ,"40" ,"40" , "41"    , "41"    , "41"    , "41"    , "42"    , "42"    , "42"    , "42"    , "42"    , "43" , "43" ,   "44"  ,   "44"  ,  "44"   ,  "44"   , "45"  , "45"  , "45"  , "45"  , "45"  , "45"  , "45"  , "45"  , "46"  , "46"  , "46"  , "46"  , "46"  , "46"  , "46"  , "46"  , "47" , "47" , "51"  , "51"  , "51"  , "51"  , "51"  , "51"  , "51"  , "51"  , "52" , "52" };
                         cli::array <String^, 2>^ marker_list = {
                                               {"MA,2,11","MA,1,13","MA,1,16","MA,2,1","MA,2,10","M,21,6","MA,1,15","MA,2,8","MA,2,9","M,18,11","M,18,12","M,19,7","M,19,6","M,19,4","M,19,3","M,19,1","M,19,2","M,18,15","M,22,3","M,15,14","MA,101,1"}, //REK1
                                               {"MA,6,11","MA,5,13","MA,5,16","MA,6,1","MA,6,10","M,31,6","MA,5,15","MA,6,8","MA,6,9","M,28,11","M,28,12","M,29,7","M,29,6","M,29,4","M,29,3","M,29,1","M,29,2","M,28,15","M,32,3","M,25,14","MA,101,2"}, //REK2
											   {"M,9,8","M,9,7","M,19,13","M,19,11","M,18,3","M,18,2","M,11,9","M,11,10","M,18,6","M,18,15"},	//BA1
                                               {"M,9,13","M,9,12","M,21,13","M,21,11","M,20,3","M,20,2","M,13,13","M,13,14","M,20,6","M,20,15"},	//BA2
                                               {"M,9,9","M,9,10","M,9,11","M,23,9","M,23,10","M,23,11","M,22,1","M,22,2","M,22,3","M,23,1","M,23,2","M,23,3","M,11,15","M,11,14","M,11,16","M,23,8"}, //VW1
                                               {"M,9,14","M,9,15","M,9,16","M,25,9","M,25,10","M,25,11","M,24,1","M,24,2","M,24,3","M,25,1","M,25,2","M,25,3","M,14,3","M,14,2","M,14,4","M,25,8"}, //VW2
                                               {"M,30,3","M,30,4","M,30,6","M,30,1","M,30,2","M,30,7","M,30,5","M,30,12","M,27,2","M,27,3","M,28,8","M,33,11","M,29,12","M,29,11"}, //ESG1M,
											   {"M,39,3","M,39,4","M,39,6","M,39,1","M,39,2","M,39,7","M,39,5","M,39,12","M,36,2","M,36,3","M,37,8","M,42,11","M,38,12","M,38,11"}, //ESG2M,
											   {"M,48,3","M,48,4","M,48,6","M,48,1","M,48,2","M,48,7","M,48,5","M,48,12","M,45,2","M,45,3","M,46,8","M,51,11","M,47,12","M,47,11"}, //ESG3M,
											   {"M,57,3","M,57,4","M,57,6","M,57,1","M,57,2","M,57,7","M,57,5","M,57,12","M,54,2","M,54,3","M,55,8","M,60,11","M,56,12","M,56,11"}, //ESG4M,
											   {"M,30,3","M,30,4","M,30,6","M,30,1","M,30,2","M,30,7","M,30,5","M,30,12","M,27,2","M,27,3","M,27,13","M,28,8","M,33,11","M,29,12","M,29,11"}, //ESG1V,
											   {"M,39,3","M,39,4","M,39,6","M,39,1","M,39,2","M,39,7","M,39,5","M,39,12","M,36,2","M,36,3","M,36,13","M,37,8","M,42,11","M,38,12","M,38,11"}, //ESG2V,
											   {"M,48,3","M,48,4","M,48,6","M,48,1","M,48,2","M,48,7","M,48,5","M,48,12","M,45,2","M,45,3","M,45,13","M,46,8","M,51,11","M,47,12","M,47,11"}, //ESG3V,
											   {"M,57,3","M,57,4","M,57,6","M,57,1","M,57,2","M,57,7","M,57,5","M,57,12","M,54,2","M,54,3","M,54,13","M,55,8","M,60,11","M,56,12","M,56,11"}, //ESG4V,
											   {"M,66,3","M,66,4","M,66,6","M,66,1","M,66,2","M,66,7","M,66,5","M,66,12","M,63,2","M,63,3","M,63,13","M,64,8","M,69,11","M,65,12","M,65,11"}, //ESG4V,
                                               {"MA,2,11","MA,1,13","MA,1,16","MA,2,1","EA,S,4","MA,1,15","M,18,11","M,18,12","M,19,7","M,19,6","M,19,4","M,19,3","M,19,1","M,19,2","M,18,15","M,22,3","M,16,13","M,16,14","M,15,14","MA,101,1"}, //RES1
                                               {"MA,6,11","MA,5,13","MA,5,16","MA,6,1","EA,S,3","MA,5,15","M,28,11","M,28,12","M,29,7","M,29,6","M,29,4","M,29,3","M,29,1","M,29,2","M,28,15","M,32,3","M,26,13","M,26,14","M,25,14","MA,101,2"}, //RES2
											   {"M,30,1","M,30,2","M,30,3","M,30,4","M,30,5","M,30,6","M,30,7","M,30,12","M,27,2","M,27,3","M,28,5","M,28,6","M,29,11","M,29,12","M,27,12","M,27,13","M,33,11","MA,101,1","MA,0,3"}, //ESG1S
											   {"M,39,1","M,39,2","M,39,3","M,39,4","M,39,5","M,39,6","M,39,7","M,39,12","M,36,2","M,36,3","M,37,5","M,37,6","M,38,11","M,38,12","M,36,12","M,36,13","M,42,11","MA,101,2","MA,0,4"}, //ESG2S
											   {"M,48,1","M,48,2","M,48,3","M,48,4","M,48,5","M,48,6","M,48,7","M,48,12","M,45,2","M,45,3","M,45,5","M,45,6","M,47,11","M,47,12","M,45,12","M,45,13","M,51,11","MA,101,3","MA,0,5"}, //ESG3S
											   {"M,57,1","M,57,2","M,57,3","M,57,4","M,57,5","M,57,6","M,57,7","M,57,12","M,54,2","M,54,3","M,54,5","M,54,6","M,56,11","M,56,12","M,54,12","M,54,13","M,60,11","MA,101,4","MA,0,6"}, //ESG3S
											   {"M,701,9","M,701,10","M,700,13","M,700,14"},	//ITE1
											   {"M,706,9","M,706,10","M,705,13","M,705,14"},	//ITE2
											   {"M,711,9","M,711,10","M,710,13","M,710,14"},	//ITE3
											   {"M,716,9","M,716,10","M,715,13","M,715,14"},	//ITE4
											   {"M,721,9","M,721,10","M,720,13","M,720,14"},	//ITE5
											   {"M,726,9","M,726,10","M,725,13","M,725,14"},	//ITE6
											   {"M,731,9","M,731,10","M,730,13","M,730,14"},	//ITE7
											   {"M,736,9","M,736,10","M,735,13","M,735,14"},	//ITE8
											   {"M,701,9","M,701,10","M,701,11","M,700,13","M,700,14","M,700,15"},	//IVL1
											   {"M,706,9","M,706,10","M,706,11","M,705,13","M,705,14","M,705,15"},	//IVL2
											   {"M,711,9","M,711,10","M,711,11","M,710,13","M,710,14","M,710,15"},	//IVL3
											   {"M,716,9","M,716,10","M,716,11","M,715,13","M,715,14","M,715,15"},	//IVL4
											   {"M,721,9","M,721,10","M,721,11","M,720,13","M,720,14","M,720,15"},	//IVL5
											   {"M,726,9","M,726,10","M,726,11","M,725,13","M,725,14","M,725,15"},	//IVL6
											   {"M,731,9","M,731,10","M,731,11","M,730,13","M,730,14","M,730,15"},	//IVL7
											   {"M,736,9","M,736,10","M,736,11","M,735,13","M,735,14","M,735,15"},	//IVL8                             
											   {"M,15,14","M,15,13","M,17,14","M,18,10","M,18,9","M,19,5"}, //RLG1
											   {"M,25,14","M,25,13","M,27,14","M,28,10","M,28,9","M,29,5"}, //RLG2
											   {"MA,50,1", "MA,60,1", "MA,51,1","MA,51,2", "MA,51,6", "MA,51,4", "MA,51,3", "M,742,15","M,742,13","M,742,12","M,700,14","M,700,13", "M,742,4", "M,742,8", "M,742,9"}, //IBR1
											   {"MA,50,3", "MA,60,3", "MA,51,9","MA,51,10","MA,51,14","MA,51,12","MA,51,11","M,744,15","M,744,13","M,744,12","M,705,14","M,705,13", "M,744,4", "M,744,8", "M,744,9"}, //IBR2
											   {"MA,50,5", "MA,60,5", "MA,52,1","MA,52,2", "MA,52,6", "MA,52,4", "MA,52,3", "M,746,15","M,746,13","M,746,12","M,710,14","M,710,13", "M,746,4", "M,746,8", "M,746,9"}, //IBR3
											   {"MA,50,7", "MA,60,7", "MA,52,9","MA,52,10","MA,52,14","MA,52,12","MA,52,11","M,748,15","M,748,13","M,748,12","M,715,14","M,715,13", "M,748,4", "M,748,8", "M,748,9"}, //IBR4
											   {"MA,50,9", "MA,60,9", "MA,53,1","MA,53,2", "MA,53,6", "MA,53,4", "MA,53,3", "M,750,15","M,750,13","M,750,12","M,720,14","M,720,13", "M,750,4", "M,750,8", "M,750,9"}, //IBR5
											   {"MA,50,11","MA,60,11","MA,53,9","MA,53,10","MA,53,14","MA,53,12","MA,53,11","M,752,15","M,752,13","M,752,12","M,725,14","M,725,13", "M,752,4", "M,752,8", "M,752,9"}, //IBR6
											   {"MA,50,13","MA,60,13","MA,54,1","MA,54,2", "MA,54,6", "MA,54,4", "MA,54,3", "M,754,15","M,754,13","M,754,12","M,730,14","M,730,13", "M,754,4", "M,754,8", "M,754,9"}, //IBR7
											   {"MA,50,15","MA,60,15","MA,54,9","MA,54,10","MA,54,14","MA,54,12","MA,54,11","M,756,15","M,756,13","M,756,12","M,735,14","M,735,13", "M,756,4", "M,756,8", "M,756,9"}, //IBR8
											   {"MA,3,4","MA,1,5","MA,1,9","MA,1,10","MA,1,7","M,20,12"}, //SWB1
											   {"MA,7,4","MA,5,5","MA,5,9","MA,5,10","MA,5,7","M,30,12"}}; //SWB2

						 int blok_beg=-1, block_end=-1;
						 for (int n_fstr=0; n_fstr<blok_list->Length; n_fstr++)
						 {
							 blok_beg = t_code->IndexOf("\n"+blok_list[n_fstr]+";\n");
							 String^ block_replace = "\n@"+ blok_com_list[n_fstr]+";\n";
							 if (blok_beg>0)
							 {
								 for (int n_rep=0; n_rep<marker_list->GetLength(1); n_rep++)
								 {
									 
									 if ( !String::IsNullOrEmpty(marker_list[n_fstr,n_rep]) )
										block_replace += "@"+ blok_com_list[n_fstr]+","+marker_list[n_fstr,n_rep]+";\n";
								 }
								 t_code = t_code->Replace("\n"+blok_list[n_fstr]+";\n", block_replace);
							 }
						 }

						 // поиск и замена команд на соответствующие коды команд
						 cli::array <String^>^ com_list = {"?","O","U","=",":","R","S","ON","UN","EX","LADB","ZUW1","ZUW0","LAD","ZUW","ADD","SUB","MUL","MAX","MIN","DIV","?","TOB","LADK","NEG","ABS","INV","RAD","TB","ASL","TA","DYN","GW","PT","INT","SPB","SMAR","SPA","?","?","?","?","?","?","?","?","?","?","U(","O(",")","?","?","KOM","SB","SBV","BEND","?","?","?","?","V,2","V,3","V,4","V,5","V,6","V,7"};
						 for (int n_fstr = com_list->Length-1; n_fstr>=0; n_fstr--)
						 {
							 rel = Convert::ToString(n_fstr);
							 t_code = t_code->Replace("\n"+com_list[n_fstr]+",", "\n@"+rel+",");
							 t_code = t_code->Replace("\n"+com_list[n_fstr]+";", "\n@"+rel+";");
						 }
						 // поиск и замена разделителей секций кода
						 t_code = t_code->Replace("\nSZYK;", "\n@100;");
						 t_code = t_code->Replace("\nLZYK;", "\n@101;");
						 t_code = t_code->Replace("\nENDE;", "\n@102;");
						 t_code = t_code->Replace("\nZYK,", "\n@103,");

						 if (i_bg!=-i_eas) 
						 {
							 // поиск и замена аналоговых маркеров ссылками на ячейки памяти
							 cli::array <String^>^ am_list = {"MA,","EA,S,","AA,S,","EA,T,","AA,T,","M,","E,S,","A,S,"};
							 cli::array <bool>^ type_list =  {true ,true   ,true   ,true   ,true   ,false,false,false };
							 for (int n_am=0; n_am<am_list->Length; n_am++)
							 {
								 do
								 {
									 finished = true;
									 int m_pos = t_code->IndexOf(am_list[n_am]);
									 int m_pos2 = t_code->IndexOf(";",m_pos+1);
									 if (m_pos>0 && m_pos2>0)
									 {
										 marker = t_code->Substring(m_pos, m_pos2-m_pos);
										 marker2 = marker->Replace("M,","M")->Replace("E,","E")->Replace("S,","S")->Replace("A,","A")->Replace("T,","T");
										 String^ newmarker = bg_short_num + "," + marker2;
										 int nmindex = -1;
										 if (type_list[n_am]) nmindex = tabrel_a[i_awe]->IndexOf(newmarker);
										 else nmindex = tabrel_b[i_awe]->IndexOf(newmarker);
										 if (nmindex > 0) 
										 {
											 rel = Convert::ToString(nmindex);
										 }
										 else
										 {
											 if (type_list[n_am])
											 {
												 rel = Convert::ToString(tabrel_a[i_awe]->Count);
												 tabrel_a[i_awe]->Add(newmarker);
												 tab_pa[i_awe]->Add(0);
											 }
											 else
											 {
												 rel = Convert::ToString(tabrel_b[i_awe]->Count);
												 tabrel_b[i_awe]->Add(newmarker);
												 tab_pb[i_awe]->Add(0);
											 }
										 }
										 t_code = t_code->Replace(marker+";", "#"+rel+";");
										 finished = false;
									 }
								 } while (!finished);
							 }
							 // поиск и замена таймеров ссылками на ячейки памяти
							 do 
							 {
								 finished = true;
								 int m_pos = t_code->IndexOf(",T,");
								 int m_pos2 = t_code->IndexOf(";",m_pos+1);
								 if (m_pos>0 && m_pos2>0)
								 {
									 marker = t_code->Substring(m_pos, m_pos2-m_pos);
									 rel = Convert::ToString(-tabrel_t[i_awe]->Count);
									 t_code = t_code->Replace(marker+";", ",#"+rel+";");
									 if (i_bg!=i_eas) 
									 {
										 marker2 = marker->Replace("T,","T");
										 tabrel_t[i_awe]->Add(bg_short_num + marker2);
									 }
									 finished = false;
								 }
							 } while (!finished);
						 }


						 // составление упрощенного кода
						 int res=0;
						 do
						 {
							res = this->step_splitter(t_code, splitlist);
							if (res>0)
							{ 
								if (splitlist->Count == 2 && splitlist[0]->Contains("@") && splitlist[1]->Contains("#") ) 
								{
									// полностью расшифрованная строка с кодом операции @  и ячейкой памяти # ( с минусом - аналоговые ячейки, с плюсом - дискретные)
									try
									{
										tab_c[i_awe]->Add(Convert::ToInt32(splitlist[0]->Substring(splitlist[0]->IndexOf("@")+1)));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_c[i_awe]->Add(0);
									}
									try
									{
										tab_m[i_awe]->Add(Convert::ToInt32(splitlist[1]->Substring(splitlist[1]->IndexOf("#")+1)));
								
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_m[i_awe]->Add(0);
									}
								}
								else if (splitlist->Count == 2 && splitlist[0]->Contains("@23") ) // LADK. бинарные данные типа float запихиваем в тот же массив типа int. Читаем потом соотв образом
								{
									try
									{
										tab_c[i_awe]->Add(Convert::ToInt32(splitlist[0]->Substring(splitlist[0]->IndexOf("@")+1)));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_c[i_awe]->Add(0);
									}
									float ladk_f=0.0f;
									try
									{
										ladk_f=Convert::ToSingle(splitlist[1]->Replace(".",this->separator));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										ladk_f=0.0f;
									}
									int ladk_i=0;
									memcpy(&ladk_i,&ladk_f,4);
									tab_m[i_awe]->Add(ladk_i);
								}
								else if (splitlist->Count == 2 && (splitlist[0]->Contains("@35") || splitlist[0]->Contains("@36") || splitlist[0]->Contains("@37"))  ) // SPB, SMAR, SPA
								{
									try
									{
										tab_c[i_awe]->Add(Convert::ToInt32(splitlist[0]->Substring(splitlist[0]->IndexOf("@")+1)));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_c[i_awe]->Add(0);
									}
									try
									{
										tab_m[i_awe]->Add(Convert::ToInt32(splitlist[1]));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_m[i_awe]->Add(0);
									}
								}
								else if (splitlist->Count == 1 && splitlist[0]->Contains("@") && splitlist[0]->Length > 1 ) // унарные операции
								{
									try
									{
										tab_c[i_awe]->Add(Convert::ToInt32(splitlist[0]->Substring(splitlist[0]->IndexOf("@")+1)));
									}
									catch(Exception^ e)
									{
										DWORD err = GetLastError( );
										this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
										tab_c[i_awe]->Add(0);
									}
									tab_m[i_awe]->Add(0);
								}
								else if (splitlist->Count == 3 && splitlist[0]->Contains("@103") )
								{
									tab_c[i_awe]->Add(103);
									tab_m[i_awe]->Add(0);
								}
								if (res+2<t_code->Length) t_code = t_code->Substring(res+2);
								else t_code="";
							 }
						 } while (res>0);

					 }
					 tab_a[i_awe] = new float[tabrel_a[i_awe]->Count+1]; // Массив аналоговых значений стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
					 tab_bba[i_awe] = new int[tabrel_a[i_awe]->Count+1]; // Массив битов-сопроводителей аналогов стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
					 tab_t[i_awe] = new float[tabrel_t[i_awe]->Count+1]; // Массив таймеров стойки. Меняется в процессе обработки кода. Позиция в списке == номер таймера в коде.
					 tab_b[i_awe] = new int[tabrel_b[i_awe]->Count+1]; // Массив дискретных значений стойки. Меняется в процессе обработки кода. Позиция в списке == номер маркера в коде.
					 for (int u=0; u<tabrel_a[i_awe]->Count; u++ ) tab_a[i_awe][u]=0.0f;
					 for (int u=0; u<tabrel_a[i_awe]->Count; u++ ) tab_bba[i_awe][u]=0;
					 for (int u=0; u<tabrel_t[i_awe]->Count; u++ ) tab_t[i_awe][u]=-100.0f;
					 for (int u=0; u<tabrel_b[i_awe]->Count; u++ ) tab_b[i_awe][u]=0;

					 tnc[i_awe]->ToolTipText += "b="+Convert::ToString(tabrel_b[i_awe]->Count)+", a="+Convert::ToString(tabrel_a[i_awe]->Count)+", t="+Convert::ToString(tabrel_t[i_awe]->Count);
					 //заполнение прогрессбара
					 emulated->is_changed[i_awe]=false;

					 for (int q=0; q<n_awe; q++)
						 if ( emulated->is_changed[q])
							  count_est++;

					 this->toolStripLabel1->Text = "Осталось обработать стоек: " + Convert::ToString(count_est);

					 if (count_est==0)
					 {
						 emulated->is_ready = true;
						 emulated->toolStripProgressBar1->Value = 0;
						 this->toolStripProgressBar1->Value = 0;
					 }
					 else
					 {
						 emulated->toolStripProgressBar1->Value = 99/(count_est+1);
						 this->toolStripProgressBar1->Value = 99/(count_est+1);
					 }
					 
					 i_awe = n_awe;
					 this->timer2->Enabled = true;
				 }


				 for (int q=0; q<n_awe; q++)
					if ( emulated->is_changed[q])
						count_est++;

				 if (count_est==0)  
				 {
					 emulated->is_ready = true;
					 emulated->toolStripProgressBar1->Value = 0;
					 this->toolStripProgressBar1->Value = 0;
					 this->timer2->Enabled = true;
				 }
			}

		 }
System::Int32 Form1::step_splitter(System::String^ str, List<String^>^ split) 
		 {
			if (split!=nullptr) split->Clear();
			else split = gcnew List<String^>();

			 int n_parts=0;
			 int dotcomma = str->IndexOf(";");
			 if (dotcomma < 0) dotcomma = str->Length;
			 if (dotcomma > 0)
			 {
				 n_parts=1;
				 int comma = str->IndexOf(",");
				 if (comma>=0 && comma<dotcomma) 
				 {
					 split->Add(str->Substring(0, comma));
					 do 
					 {
						 n_parts++;
						 int oldcomma = comma;
						 comma = str->IndexOf(",",comma+1); 
						 if (comma>0 && comma<dotcomma) split->Add(str->Substring(oldcomma+1, comma-oldcomma-1));
						 else split->Add(str->Substring(oldcomma+1, dotcomma-oldcomma-1));
					 }
					 while (comma>0 && comma<dotcomma);
				 }
				 else
				 {
					 split->Add(str->Substring(0, dotcomma));
				 } 
			 }
			 return dotcomma;
		 }

System::Void Form1::timer2_Tick(System::Object^  sender, System::EventArgs^  e) 
 {
	 SYSTEMTIME u;
	 GetLocalTime(&u);
	 char f[24] = "dd.mm.YYYY HH:MM:SS,xms";

	 unsigned long t_start=0;
	 unsigned long t_finish=0;
	 t_start = GetTickCount();

	 sprintf(&f[0], "%02d.%02d.%4d %02d:%02d:%02d,%03d", u.wDay, u.wMonth, u.wYear, u.wHour, u.wMinute, u.wSecond, u.wMilliseconds );

	 int n_awe = tnc->Count;
	 if (n_awe<0) n_awe=0;

for (int i_awe=0; i_awe<n_awe; i_awe++)
	try
	{ 
	 //tab_c[i_awe]->Count;
	 if (emulated->state[i_awe] && !emulated->is_changed[i_awe])
	 {
		 List<int>^ tlist_m = tab_m[i_awe];
		 List<int>^ tlist_c = tab_c[i_awe];
		 bool VKE = false, VKE0 = false;
		 bool ERAB = true, ERAB0 = true;
		 bool prO = false, prU = false;
		 float AKKU = 0.0f;
		 float cycle = 0.1f;
		 bool slzyk = false;
		 bool easzyk = true, mem_easzyk = true;
		 bool imit = false;
		 int mark_n = 1;
		 char *val;
		 char *descr; 

		 

		 for (int i_com=0; i_com<tab_c[i_awe]->Count; i_com++)
		 {
			 int mem = tlist_m[i_com], mem2=0, mem3=0, mem4=0, mem5=0, mem6=0, mem7=0;
			 int memesg[21];
			 int com = tlist_c[i_com];
			 
			 if ( (mem_easzyk && !easzyk)  ||  (!mem_easzyk && easzyk) ) // Момент для ввода имитаций - до и после обработки кода EAS в каждой стойке
			 {
				 try
				 {
					 if (debugger->dataGridView1->RowCount>0)
					 {
						 for (int i_row=0; i_row<debugger->dataGridView1->RowCount-1; i_row++) // запись значений в маркеры
						 {
							 try
							 {
								 String^ c_awe = debugger->dataGridView1[0,i_row]->Value->ToString();
								 String^ c_marker = debugger->dataGridView1[1,i_row]->Value->ToString();
								 if (c_awe != "" && c_marker != "" && tnc[i_awe]->Text->Contains(c_awe) )
								 {
									 if(debugger->dataGridView1[3,i_row]->Value->ToString() != "" && debugger->dataGridView1[4,i_row]->Value->ToString() == "True" ) // установить
									 { 
										 String^ c_value = debugger->dataGridView1[3,i_row]->Value->ToString();
										 if (c_value != "")
										 {
											 // нужно пройтись по коду и заменить адреса в ячейках, где есть процедуры чтения 
											 String^ res = this->SetMarkerValue(c_awe, c_marker, c_value);
										 }
									 }
								 } 
							 }
							 catch (Exception^ ex) 
							 {
								DWORD err = GetLastError( );
								this->toolStripLabel1->Text = ex->Message + Convert::ToString((int)err);
							 }
						 }

						 for (int i_row=0; i_row<debugger->dataGridView1->RowCount-1; i_row++) // чтение значений из маркеров
						 {
							 try
							 {
								 String^ c_awe = debugger->dataGridView1[0,i_row]->Value->ToString();
								 String^ c_marker = debugger->dataGridView1[1,i_row]->Value->ToString();
								 if (c_awe != "" && c_marker != "" && tnc[i_awe]->Text->Contains(c_awe) )
								 {
									 int color;
									 debugger->dataGridView1[2,i_row]->Value = this->GetMarkerValue(c_awe, c_marker, &color);
								 } 
							 }
							 catch (Exception^ ex) 
							 {
								DWORD err = GetLastError( );
								this->toolStripLabel1->Text = ex->Message + Convert::ToString((int)err);
							 };
						 }
					 }
					 else
					 {
						this->DebuggerButton_Click(sender, e); 
						this->DebuggerButton->BackColor = Color::WhiteSmoke;
					 }
				 }
				 catch (Exception^ ex) 
				 {
					 DWORD err = GetLastError( );
					 this->toolStripLabel1->Text = ex->Message + Convert::ToString((int)err);
					 //this->DebuggerButton_Click(sender, e); 
					 this->DebuggerButton->BackColor = Color::WhiteSmoke;
				 };

				 // обработка пересылок из других шкафов
				 for (int mem_b=0; mem_b<tabrel_b[i_awe]->Count; mem_b++)
				 {
 					if (tabrel_b[i_awe][mem_b]->StartsWith("KK")) // если сигнал нужно забрать из другого шкафа
					{
						for (int part_awe=0; part_awe<n_awe; part_awe++)
						if (part_awe!=i_awe && emulated->state[part_awe] && !emulated->is_changed[part_awe]) // пройдемся по всем другим шкафам
						{
							try
							{
								int mem_result = tabrel_b[part_awe]->IndexOf("EK"+tabrel_b[i_awe][mem_b]->Substring(2));
								if (mem_result>=0) // нашелся нужный бит нужной телеграммы в другом шкафу
								{
									tab_b[i_awe][mem_b] = tab_b[part_awe][mem_result];
									part_awe = n_awe;
								}
							}
							catch(...)
							{
							}
						}
					}
				 }
				 for (int mem_a=0; mem_a<tabrel_a[i_awe]->Count; mem_a++)
				 {
 					if (tabrel_a[i_awe][mem_a]->StartsWith("KK")) // если сигнал нужно забрать из другого шкафа
					{
						for (int part_awe_a=0; part_awe_a<n_awe; part_awe_a++)
						if (part_awe_a!=i_awe && emulated->state[part_awe_a] && !emulated->is_changed[part_awe_a]) // пройдемся по всем другим шкафам
						{
							try
							{
								int mem_result_a = tabrel_a[part_awe_a]->IndexOf("EK"+tabrel_a[i_awe][mem_a]->Substring(2));
								if (mem_result_a>=0) // нашелся нужный бит нужной телеграммы в другом шкафу
								{
									tab_a[i_awe][mem_a] = tab_a[part_awe_a][mem_result_a];
									part_awe_a = n_awe;
								}
							}
							catch(...)
							{
							}
						}
					}
				 }

			 }


			 mem_easzyk = easzyk;

			 try{

			 switch (com)
			 {
				case 100: // SZYK
					VKE = false;
					ERAB = true;
					slzyk = true;      // A-cycle: slzyk = 0 and easzyk == 0 
					easzyk = false;    // S\L-cycle: slzyk = 1 and easzyk == 0 
					break;             // EAS-cycle: slzyk = 0 and easzyk == 1 
				
				case 101: // LZYK
					VKE = false;
					ERAB = true;
					slzyk = true;
					easzyk = false;
					break;
						
				case 102: // ENDE - после него пойдет А-цикл следующего модуля
					VKE = false;
					ERAB = true;
					slzyk = false;
					easzyk = false;
					break;

				case 103: // ZYK - секция обработки кода eas
					VKE = false;
					ERAB = true;
					slzyk = false;
					easzyk = true;
					break;

				default:

					break;
			 }
			 if ( easzyk && this->StopButton->Enabled) // циклическая обработка кода eas
			 {

				 switch (com)
				 {
					case 4: // :
						if (tab_b[i_awe][mem]!=VKE && tab_pb[i_awe][mem]<0 ) 
						{
							val = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tabrel_b[i_awe][0]+","+tabrel_b[i_awe][mem]).ToPointer();
							if (archive!=NULL) fprintf(archive, "%s\t%s\t%s\t%d->%d\n", f, descr, val, tab_b[i_awe][mem], VKE);
						}
						step_CO(&tab_b[i_awe][mem],VKE);
						break;

					case 10: // LADB
						if (tab_pb[i_awe][mem]>0) // если у дискретного сигнала есть описание (расширение)
						{
							descr = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tabarh_b[i_awe][tab_pb[i_awe][mem]]).ToPointer();
						}
						step_LADB(tab_b[i_awe][mem],&VKE,&ERAB);												
						break;

					case 13: // LAD
						if (tab_pa[i_awe][mem]>0) // если у аналогового сигнала есть описание (расширение)
						{
							descr = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tabarh_a[i_awe][tab_pa[i_awe][mem]]).ToPointer();
						}
						step_LAD(tab_a[i_awe][mem],&AKKU);
						break;

					case 14: // ZUW
						if (tab_a[i_awe][mem]!=AKKU && !Single::IsNaN(AKKU) && !Single::IsInfinity(AKKU) && tab_pa[i_awe][mem]<0)
						{
							val = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tabrel_a[i_awe][0]+","+tabrel_a[i_awe][mem]).ToPointer();
							if (archive!=NULL) fprintf(archive, "%s\t%s\t%s\t%f\n", f, descr, val, AKKU);
						}
						step_ZUW(&tab_a[i_awe][mem],AKKU);
						break;

					default:

						break;
				 }
			 }
			 else if (!easzyk && ( slzyk || (!emulated->old_state[i_awe] && emulated->state[i_awe])) )  // циклическая обработка кодов модулей и 1 раз при первом проходе таймера цикла инициализации
			 {
				 switch (com)
				 {
					case 1: // O
						if (mem>0) step_O(tab_b[i_awe][mem],&VKE,&ERAB);
						else if (mem<0) step_O(tab_t[i_awe][-mem],AKKU,&VKE,&ERAB);
						break;

					case 2: // U
						if (mem>0) step_U(tab_b[i_awe][mem],&VKE,&ERAB);
						else if (mem<0) step_U(tab_t[i_awe][-mem],AKKU,&VKE,&ERAB);
						break;

					case 3: // =
						step_EQ(&tab_b[i_awe][mem],VKE,&ERAB);
						break;

					case 4: // :
						step_CO(&tab_b[i_awe][mem],VKE);
						break;

					case 5: // R
						if (mem>0) step_R(&tab_b[i_awe][mem],VKE,&ERAB);
						else if (mem<0) step_R(&tab_t[i_awe][-mem],VKE,&ERAB);							
						break;
					
					case 6: // S
						if (mem>0) step_S(&tab_b[i_awe][mem],VKE,&ERAB);
						else if (mem<0) step_S(&tab_t[i_awe][-mem],AKKU,VKE,&ERAB);														
						break;

					case 7: // ON
						if (mem>0) step_ON(tab_b[i_awe][mem],&VKE,&ERAB);
						else if (mem<0) step_ON(tab_t[i_awe][-mem],AKKU,&VKE,&ERAB);														
						break;

					case 8: // UN
						if (mem>0) step_UN(tab_b[i_awe][mem],&VKE,&ERAB);
						else if (mem<0) step_UN(tab_t[i_awe][-mem],AKKU,&VKE,&ERAB);														
						break;

					case 9: // EX
						step_EX(tab_b[i_awe][mem],&VKE,&ERAB);													
						break;

					case 10: // LADB
						step_LADB(tab_b[i_awe][mem],&VKE,&ERAB);												
						break;

					case 11: // ZUW1
						step_ZUW1(&tab_b[i_awe][mem]);
						break;

					case 12: // ZUW0
						step_ZUW0(&tab_b[i_awe][mem]);
						break;

					case 13: // LAD
						step_LAD(tab_a[i_awe][mem],&AKKU);
						break;

					case 14: // ZUW
						step_ZUW(&tab_a[i_awe][mem],AKKU);
						break;

					case 15: // ADD
						step_ADD(tab_a[i_awe][mem],&AKKU);
						break;

					case 16: // SUB
						step_SUB(tab_a[i_awe][mem],&AKKU);
						break;

					case 17: // MUL
						step_MUL(tab_a[i_awe][mem],&AKKU);
						break;

					case 18: // MAX
						step_MAX(tab_a[i_awe][mem],&AKKU,&VKE,&ERAB);
						break;

					case 19: // MIN
						step_MIN(tab_a[i_awe][mem],&AKKU,&VKE,&ERAB);
						break;

					case 20: // DIV
						step_DIV(tab_a[i_awe][mem],&AKKU);
						break;

					case 21: // 

						break;

					case 22: // TOB
						step_TOB(tab_a[i_awe][mem],&AKKU);
						break;

					case 23: // LADK
						step_LADK(&mem,&AKKU);
						break;

					case 24: // NEG
						step_NEG(&VKE,&ERAB);
						break;

					case 25: // ABS
						step_ABS(&AKKU);
						break;

					case 26: // INV
						step_INV(&AKKU);
						break;

					case 27: // RAD
						step_RAD(&AKKU);
						break;

					case 28: // TB
						i_com++; mem2 = tlist_m[i_com];
						step_TB(&tab_b[i_awe][mem],tab_b[i_awe][mem2]);
						break;

					case 29: // ASL
						i_com++; mem2 = tlist_m[i_com];
						step_ASL(tab_a[i_awe][mem],tab_b[i_awe][mem2],&AKKU);
						break;

					case 30: // TA
						i_com++; mem2 = tlist_m[i_com];
						step_TA(&tab_a[i_awe][mem],tab_a[i_awe][mem2]);
						break;

					case 31: // DYN
						i_com++; mem2 = tlist_m[i_com];
						step_DYN(&tab_b[i_awe][mem],tab_b[i_awe][mem2],&VKE,&ERAB);
						break;

					case 32: // GW
						i_com++; mem2 = tlist_m[i_com];
						i_com++; mem3 = tlist_m[i_com];
						step_GW(&tab_b[i_awe][mem],tab_a[i_awe][mem2],tab_a[i_awe][mem3],AKKU,&VKE,&ERAB);
						break;

					case 33: // PT
						i_com++; mem2 = tlist_m[i_com];
						i_com++; mem3 = tlist_m[i_com];
						if (!emulated->old_state[i_awe] && emulated->state[i_awe]) step_PT(&tab_a[i_awe][mem],tab_a[i_awe][mem2],1,cycle,&AKKU);
						else step_PT(&tab_a[i_awe][mem],tab_a[i_awe][mem2],tab_b[i_awe][mem3],cycle,&AKKU);
						break;

					case 34: // INT
						i_com++; mem2 = tlist_m[i_com];
						i_com++; mem3 = tlist_m[i_com];
						i_com++; mem4 = tlist_m[i_com];
						i_com++; mem5 = tlist_m[i_com];
						i_com++; mem6 = tlist_m[i_com];
						i_com++; mem7 = tlist_m[i_com];
						step_INT(
							&tab_a[i_awe][mem],
							tab_a[i_awe][mem2],
							tab_a[i_awe][mem3],
							tab_a[i_awe][mem4],
							tab_a[i_awe][mem5],
							tab_b[i_awe][mem6],
							tab_b[i_awe][mem7],cycle,&AKKU);
						break;

					case 35: // SPB
						mark_n = mem;
						if (VKE) 
						{
							do
							{
								i_com++;
								mem = tlist_m[i_com];
								com = tlist_c[i_com];
							} while ( (com!=36 || mem!=mark_n) && com!=100 && com!=101 && com!=102);
						}
						else
						{
							VKE = true;
							ERAB = true;
						}
						break;

					case 36: // SMAR

						break;

					case 37: // SPA
						mark_n = mem;
						do
						{
							i_com++;
							mem = tlist_m[i_com];
							com = tlist_c[i_com];
						} while ( (com!=36 || mem!=mark_n) && com!=100 && com!=101 && com!=102);
						break;

					case 38: // REK
                        for (int h = 0; h < 21; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_REK(
                            &tab_a[i_awe][memesg[0]],	//ma,2,11	ВЫХОД
                            tab_a[i_awe][memesg[1]],	//ma,1,13	РАССОГЛАСОВАНИЕ
                            tab_a[i_awe][memesg[2]],	//ma,1,16	Кп
                            tab_a[i_awe][memesg[3]],	//ma,2,1	Ти
                            tab_a[i_awe][memesg[4]],	//ma,2,10	yn
                            tab_b[i_awe][memesg[5]],	//m,21,6	ryn
                            tab_a[i_awe][memesg[6]],	//ma,1,15	Tхода
							tab_a[i_awe][memesg[7]],	//ma,2,9	Макс
							tab_a[i_awe][memesg[8]],	//ma,2,8	Мин
                            tab_b[i_awe][memesg[9]],	//m,18,11	авт закр
                            tab_b[i_awe][memesg[10]],	//m,18,12	авт откр
                            tab_b[i_awe][memesg[11]],	//m,19,7	руч закр
                            tab_b[i_awe][memesg[12]],	//m,19,6	руч откр
                            tab_b[i_awe][memesg[13]],	//m,19,4	защ закр
                            tab_b[i_awe][memesg[14]],	//m,19,3	закщ откр
                            tab_b[i_awe][memesg[15]],	//m,19,1	разр закр
                            tab_b[i_awe][memesg[16]],	//m,19,2	разр откр
                            tab_b[i_awe][memesg[17]],	//m,18,15	разрРегЗакр
                            tab_b[i_awe][memesg[18]],	//m,22,3	разрРегОткр
                            tab_b[i_awe][memesg[19]],	//m,15,14	АУ
                            &tab_a[i_awe][memesg[20]],	//Исост
                            &AKKU, cycle);
                        break;

                   case 39: // BA
                        for (int h = 0; h < 10; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_BA(
                            tab_b[i_awe][memesg[0]],	//m,9,8
                            tab_b[i_awe][memesg[1]],	//m,9,7
                            tab_b[i_awe][memesg[2]],	//m,19,13
                            tab_b[i_awe][memesg[3]],	//m,19,11
                            &tab_b[i_awe][memesg[4]],	//m,18,3
                            &tab_b[i_awe][memesg[5]],	//m,18,2
                            tab_b[i_awe][memesg[6]],	//m,11,9
                            tab_b[i_awe][memesg[7]],	//m,11,10
                            &tab_b[i_awe][memesg[8]],	//m,18,6 tlsb признак мигания
                            &tab_b[i_awe][memesg[9]]);	//m,18,15
                        break;
                  
                   case  40: // VW
                        for (int h = 0; h < 16; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_VW(
                            tab_b[i_awe][memesg[0]],	//m,9,9
                            tab_b[i_awe][memesg[1]],	//m,9,10
                            tab_b[i_awe][memesg[2]],	//m,9,11
                            tab_b[i_awe][memesg[3]],	//m,23,9
                            tab_b[i_awe][memesg[4]],	//m,23,10
                            tab_b[i_awe][memesg[5]],	//m,23,11
                            &tab_b[i_awe][memesg[6]],	//m,22,1
                            &tab_b[i_awe][memesg[7]],	//m,22,2
                            &tab_b[i_awe][memesg[8]],	//m,22,3
                            &tab_b[i_awe][memesg[9]],	//m,23,1
                            &tab_b[i_awe][memesg[10]],	//m,23,2
                            &tab_b[i_awe][memesg[11]],	//m,23,3
                            tab_b[i_awe][memesg[12]],	//m,11,15
                            tab_b[i_awe][memesg[13]],	//m,11,14
                            tab_b[i_awe][memesg[14]],	//m,11,16
                            tab_b[i_awe][memesg[15]]);	//m,23,8
                        break;

					case 41: // ESG,M
						for (int h=0; h<14; h++) {i_com++; memesg[h] = tlist_m[i_com];}
						step_ESGM(
							tab_b[i_awe][memesg[0]],	//m,30,3,
							tab_b[i_awe][memesg[1]],	//m,30,4,
							tab_b[i_awe][memesg[2]],	//m,30,6,
							tab_b[i_awe][memesg[3]],	//m,30,1,
							tab_b[i_awe][memesg[4]],	//m,30,2,
							tab_b[i_awe][memesg[5]],	//m,30,7,
							tab_b[i_awe][memesg[6]],	//m,30,5,
							tab_b[i_awe][memesg[7]],	//m,30,12,
							&tab_b[i_awe][memesg[8]],	//m,27,2,
							&tab_b[i_awe][memesg[9]],	//m,27,3,
							&tab_b[i_awe][memesg[10]],	// s1av,
							&tab_b[i_awe][memesg[11]],	// azs,
							tab_b[i_awe][memesg[12]],	//m,29,12
							tab_b[i_awe][memesg[13]]);	//m,29,11
						break;

					case 42: // ESG,V
						for (int h=0; h<15; h++) {i_com++; memesg[h] = tlist_m[i_com];}
						step_ESGV(
							tab_b[i_awe][memesg[0]],	//m,30,3,
							tab_b[i_awe][memesg[1]],	//m,30,4,
							tab_b[i_awe][memesg[2]],	//m,30,6,
							tab_b[i_awe][memesg[3]],	//m,30,1,
							tab_b[i_awe][memesg[4]],	//m,30,2,
							tab_b[i_awe][memesg[5]],	//m,30,7,
							tab_b[i_awe][memesg[6]],	//m,30,5,
							tab_b[i_awe][memesg[7]],	//m,30,12,
							&tab_b[i_awe][memesg[8]],	//m,27,2,
							&tab_b[i_awe][memesg[9]],	//m,27,3,
							&tab_b[i_awe][memesg[10]],	// aloe,
							&tab_b[i_awe][memesg[11]],	// s1av,
							&tab_b[i_awe][memesg[12]],	// azs,
							tab_b[i_awe][memesg[13]],	//m,29,12
							tab_b[i_awe][memesg[14]]);	//m,29,11
						break;
					
					case 43: // RES
                        for (int h = 0; h < 20; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_RES(
                            &tab_a[i_awe][memesg[0]],  //ma,2,11	ВЫХОД
                            tab_a[i_awe][memesg[1]],   //ma,2,13	РАССОГЛАСОВАНИЕ
                            tab_a[i_awe][memesg[2]],   //ma,2,16	Кп
                            tab_a[i_awe][memesg[3]],   //ma,2,1		Ти
                            &tab_a[i_awe][memesg[4]],  //ea,s,4		УП
                            tab_a[i_awe][memesg[5]],   //ma,1,15	Tхода
                            tab_b[i_awe][memesg[6]],   //m,18,11	авт закр
                            tab_b[i_awe][memesg[7]],   //m,18,12	авт откр
                            tab_b[i_awe][memesg[8]],   //m,19,7	руч закр
                            tab_b[i_awe][memesg[9]],   //m,19,6	руч откр
                            tab_b[i_awe][memesg[10]],  //m,19,4	защ закр
                            tab_b[i_awe][memesg[11]],  //m,19,3	закщ откр
                            tab_b[i_awe][memesg[12]],  //m,19,1	разр закр
                            tab_b[i_awe][memesg[13]],  //m,19,2	разр откр
                            tab_b[i_awe][memesg[14]],  //m,18,15	разрРегЗакр
                            tab_b[i_awe][memesg[15]],  //m,22,3	разрРегОткр
                            &tab_b[i_awe][memesg[16]], //m,16,13	ARZU
                            &tab_b[i_awe][memesg[17]], //m,16,14	ARAF
                            tab_b[i_awe][memesg[18]],  //m,15,14	Ау
                            &tab_a[i_awe][memesg[19]], //Исост
                            &AKKU, cycle);
                        break;

					case 44: // ESG,S
                        for (int h = 0; h < 19; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_ESGS(
                            tab_b[i_awe][memesg[0]],   //m,30,1
                            tab_b[i_awe][memesg[1]],   //m,30,2
                            tab_b[i_awe][memesg[2]],   //m,30,3
                            tab_b[i_awe][memesg[3]],   //m,30,4
                            tab_b[i_awe][memesg[4]],   //m,30,5
                            tab_b[i_awe][memesg[5]],   //m,30,6
                            tab_b[i_awe][memesg[6]],   //m,30,7
                            tab_b[i_awe][memesg[7]],   //m,30,12
                            &tab_b[i_awe][memesg[8]],  //m,27,2 
                            &tab_b[i_awe][memesg[9]], //m,27,3
                            &tab_b[i_awe][memesg[10]], //m,28,5
                            &tab_b[i_awe][memesg[11]], //m,28,6
                            tab_b[i_awe][memesg[12]],  //m,29,11
                            tab_b[i_awe][memesg[13]],  //m,29,12
                            &tab_b[i_awe][memesg[14]], // М,27,12 ALS
                            &tab_b[i_awe][memesg[15]], // М,27,13 ALOE
							&tab_b[i_awe][memesg[16]], // AZS
                            &tab_a[i_awe][memesg[17]], //положение задвижки
							tab_a[i_awe][memesg[18]], //Тхода
                            cycle); //BHEVO BHAVO
                        break;

                   case 45: // ITE
                        for (int h = 0; h < 4; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_ITE(
                            tab_b[i_awe][memesg[0]],	//HBH
                            tab_b[i_awe][memesg[1]],	//HBA
                            &tab_b[i_awe][memesg[2]],	//*H
                            &tab_b[i_awe][memesg[3]]);	//*A
                        break;
                  
                   case  46: // IVL
                        for (int h = 0; h < 6; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_IVL(
                            tab_b[i_awe][memesg[0]],	//HVW1
                            tab_b[i_awe][memesg[1]],	//HVW2
                            tab_b[i_awe][memesg[2]],	//HVW3
                            &tab_b[i_awe][memesg[3]],	//*VW1
                            &tab_b[i_awe][memesg[4]],	//*VW2
                            &tab_b[i_awe][memesg[5]]);	//*VW3
                        break;

					case 47: // RLG
                        for (int h = 0; h < 6; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
                        step_RLG(
                            &tab_b[i_awe][memesg[0]],	//m,15,14	АУ
                            &tab_b[i_awe][memesg[1]],	//m,15,13	ДУ
                            &tab_b[i_awe][memesg[2]],	//m,17,14	КомАуДу
                            tab_b[i_awe][memesg[3]],	//m,18,10	АвтКомАУ
                            tab_b[i_awe][memesg[4]],	//m,18,9	АвтКомДу
                            &tab_b[i_awe][memesg[5]]);	//m,19,5	TDD
                        break;

					case 48: // U(
						VKE0 = VKE;
						ERAB0 = ERAB;
						prU = true;
						ERAB = true;
						break;

					case 49: // O(
						VKE0 = VKE;
						ERAB0 = ERAB;
						prO = true;
						ERAB = true;
						break;

					case 50: // )
						if (ERAB0 == false)
						{
							if (prU == true)
								VKE = VKE & VKE0;
							if (prO == true)
								VKE = VKE | VKE0;
						}
						prO = false;
						prU = false;
						break;
					
					case 51: // IBR
						for (int h = 0; h < 15; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
						step_IBR(
							&tab_a[i_awe][memesg[0]],				//ma,50,1 ma,50,3... задание
							&tab_a[i_awe][memesg[1]],				//ma,60,1 ma,60,3... выход
							tab_a[i_awe][memesg[2]],			//ma,51,1 ma,51,9 ma,52,1 .... YN
							tab_a[i_awe][memesg[3]],		//ma,51,2 ma,51,10 ma,52,2 .... X	
							tab_a[i_awe][memesg[4]],		//ma,51,6 ma,51,14 ma,52,6 .... Тхода
							tab_a[i_awe][memesg[5]],		//ma,51,4 ma,51,12 ma,52,4 .... Мин
							tab_a[i_awe][memesg[6]],		//ma,51,3 ma,51,11 ma,52,3 .... Макс
							tab_b[i_awe][memesg[7]],				// m,742,15 m,744,15... YNF
							tab_b[i_awe][memesg[8]],				// m,742,13 m,744,13... РазрЗакр
							tab_b[i_awe][memesg[9]],				// m,742,12 m,744,12... РазрОткр
							&tab_b[i_awe][memesg[10]],				// m,700,14 m,705,14... *Ау
							&tab_b[i_awe][memesg[11]],				// m,700,13 m,705,13... *Ду
							&tab_b[i_awe][memesg[12]],				// КомАуДу
							tab_b[i_awe][memesg[13]],					// АвтКомАу
							tab_b[i_awe][memesg[14]],					// АвтКомДу
							&AKKU,
							cycle);	//
						break;

					case 52: // SWB
						for (int h = 0; h < 6; h++) { i_com++; memesg[h] = tlist_m[i_com]; }
						step_SWB(
							&tab_a[i_awe][memesg[0]],		//*WHT задание СВБУ. ma,3,4
							&tab_a[i_awe][memesg[1]],		//*WW выход ma,1,5
							tab_a[i_awe][memesg[2]],		//OG ma,1,9
							tab_a[i_awe][memesg[3]],		//UG ma,1,10
							tab_a[i_awe][memesg[4]],		//WN отслеживаемая ma,1,7
							tab_b[i_awe][memesg[5]],		//WNF слежение m,20,12
							&AKKU);
						break;

					case 53: // KOM

						break;

					case 54: // SB

						break;
					
					case 55: // SBV

						break;

					case 56: // BEND

						break;


					default:

						break;
				}


			}

			}
			catch (Exception^ e) 
			{
				DWORD err = GetLastError( );
				this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);

				fprintf(archive, "i_awe=%d, i_com=%d, com=%d, mem=%d. Error\n", i_awe, i_com, com, mem);
			};
		 }

		 for (int i_t=0; i_t<tabrel_t[i_awe]->Count; i_t++)
		 {
			 if (tab_t[i_awe][i_t]>0) tab_t[i_awe][i_t] -= cycle;
		 }

		 emulated->old_state[i_awe] = emulated->state[i_awe];


	 }
	}
	catch (Exception^ e) 
	{
		DWORD err = GetLastError( );
		this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
	};

	 t_finish = GetTickCount();

	 this->StopButton->Enabled = true;
	 this->PauseButton->Enabled = true;
	 
	 this->toolStripLabel1->Text = Convert::ToString( (int)(t_finish)-(int)(t_start))+"ms";
	 if (this->SearchResults->SelectedIndex >=0) this->SearchResults_SelectedIndexChanged(sender,e);
		

	 //если в процессе работы закрыть окно эмулируемых стоек, то эмуляция остановится
	 if (emulated->is_closed == 1) 
	 {
		 DWORD err = GetLastError( );
		 this->toolStripLabel1->Text = /*ex->Message +*/ Convert::ToString((int)err);
		 this->StopButton_Click(sender,e);
	 }
	 else
	 {
		 // проверим, не поменялось ли что-то с выбором стоек?
		 this->PlayButton_Click(sender,e);
		 if (emulated->is_closed == 2) this->EmulatorButton->BackColor = Color::WhiteSmoke;
		 if (emulated->is_closed == 0) this->EmulatorButton->BackColor = Color::SkyBlue;
		 if (debugger->is_closed == 2) this->DebuggerButton->BackColor = Color::WhiteSmoke;
		 if (debugger->is_closed == 0) this->DebuggerButton->BackColor = Color::SkyBlue;
	 }

 }
System::Void Form1::StopButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->timer2->Enabled = false;
			 this->StopButton->Enabled = false;
			 this->PauseButton->Enabled = false;
			 this->PlayButton->Enabled = true;
			 this->DebuggerButton->Enabled = false;
			 this->EmulatorButton->Enabled = false;
			 this->EmulatorButton->BackColor = Color::WhiteSmoke;
			 try
			 {
				 emulated->Close();
			 }
			 catch (Exception^ e) 
			 {
				DWORD err = GetLastError( );
				this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
			 }
			 try
			 {
				 debugger->Close();
			 }
			 catch (Exception^ e) 
			 {
				DWORD err = GetLastError( );
				this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
			 }
			 try
			 {
				 std::fclose(archive);
			 }
			 catch (Exception^ e) 
			 {
				DWORD err = GetLastError( );
				this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
			 }
			 
		 }
System::Void Form1::PauseButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->timer2->Enabled = false;
			 this->PauseButton->Enabled = false;
			 this->PlayButton->Enabled = true;			 
			 std::fclose(archive);
		 }

System::String^ Form1::GetMarkerValue(System::String^ awe, System::String^ marker, int* color) 
		 {
		    int m_res=-1;
			int result_b=-1;
			float result_a=0.0f;

			int n_awe = tnc->Count;
			if (n_awe<0) n_awe=0;

			for (int i_awe=0; i_awe<n_awe; i_awe++)
			if (emulated->old_state[i_awe] && !emulated->is_changed[i_awe] && tabrel_b[i_awe][0]->Contains("("+awe+")"))
			{
				m_res = tabrel_b[i_awe]->IndexOf(marker);
				if (m_res>=0)
				{
					result_b = tab_b[i_awe][m_res];
					if (result_b == 1) *color = 2; // лаймовый цвет = сработаный дискрет	
					else *color = 1; // синий цвет = несработаный дискрет 
					return Convert::ToString(result_b);
				}
				else 
				{
					m_res = tabrel_a[i_awe]->IndexOf(marker);
					if (m_res>=0)
					{
						result_a = tab_a[i_awe][m_res];
						*color = 3; // фиолетовый цвет = аналоговое значение
						return Convert::ToString(result_a);
					}
					else
					{
						m_res = tabrel_t[i_awe]->IndexOf(marker);
						if (m_res>=0)
						{
							result_a = tab_t[i_awe][m_res];
							if (result_a <= 0.0f) *color = 1; // синий цвет = несработаный таймер
							else *color = 2; // зеленый цвет = сработаный таймер
							if (result_a < -99.0f) return "Res("+Convert::ToString(result_a)+")";
							else return Convert::ToString(result_a);
						}
						else
						{
							*color = -1;
							return ""; // ничего не нашлось
						}
					}
				}
			}
			return ""; // ничего не нашлось
		 }

System::Void* Form1::GetMarkerAddress(System::String^ awe, System::String^ marker) 
		 {
		    int m_res=-1;

			int n_awe = tnc->Count;
			if (n_awe<0) n_awe=0;

			for (int i_awe=0; i_awe<n_awe; i_awe++)
			if (emulated->old_state[i_awe] && !emulated->is_changed[i_awe] && tabrel_b[i_awe][0]->Contains("("+awe+")"))
			{
				m_res = tabrel_b[i_awe]->IndexOf(marker);
				if (m_res>=0) return &tab_b[i_awe][m_res]; // это дискрет
				else 
				{
					m_res = tabrel_a[i_awe]->IndexOf(marker);
					if (m_res>=0) return &tab_a[i_awe][m_res]; // это аналог
					else
					{
						m_res = tabrel_t[i_awe]->IndexOf(marker);
						if (m_res>=0) return &tab_t[i_awe][m_res];
						else return 0; // ничего не нашлось
					}
				}
			}
			return 0; // ничего не нашлось
		 }

System::String^ Form1::SetMarkerValue(System::String^ awe, System::String^ marker, System::String^ value) 
		 {
		    int m_res=-1;
			int result_b=0;
			float result_a=0.0f;

			int n_awe = tnc->Count;
			if (n_awe<0) n_awe=0;

			for (int i_awe=0; i_awe<n_awe; i_awe++)
			if (emulated->old_state[i_awe] && !emulated->is_changed[i_awe] && tabrel_b[i_awe][0]->Contains("("+awe+")"))
			{
				m_res = tabrel_b[i_awe]->IndexOf(marker);
				if (m_res>0)
				{
					try
					{
						result_b = Convert::ToInt32(value);
						tab_b[i_awe][m_res] = result_b;
						return "success,1";
					}
					catch (...)
					{
						return "fault,0";
					}
				}
				else 
				{
					m_res = tabrel_a[i_awe]->IndexOf(marker);
					if (m_res>0)
					{
						try
						{
							result_a = Convert::ToSingle(value);
							tab_a[i_awe][m_res]= result_a;
							return "success,1";
						}
						catch (...)
						{
							return "fault,0";
						}
					}
					else
					{
						m_res = tabrel_t[i_awe]->IndexOf(marker);
						if (m_res>0)
						{
							try
							{
								result_a = Convert::ToSingle(value);
								tab_t[i_awe][m_res]= result_a;
								return "success,1";
							}
							catch (...)
							{
								return "fault,0";
							}
						}
						else
						{
							return "fault,0"; // ничего не нашлось
						}
					}
				}
			}
			return "fault,0";
		 }

System::Void Form1::contextMenuStrip1_ItemClicked(System::Object^ sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e)
{
	String^ new_awe = gcnew String(info.awenum);
	String^ new_bg = gcnew String(info.bgnum);
	String^ new_marker = "";
	String^ ClickedItemText;
	String^ ClickedItemToolTipText;
	
	if (sender=="DoubleClick") 
	{
		ClickedItemText = this->contextMenuStrip1->Items[0]->Text;
		ClickedItemToolTipText = this->contextMenuStrip1->Items[0]->ToolTipText;
	}
	else 
	{
		ClickedItemText = e->ClickedItem->Text;
		ClickedItemToolTipText = e->ClickedItem->ToolTipText;
	}


	new_marker = new_bg+","+ClickedItemText;
	new_marker = new_marker->ToUpper();
	new_marker = new_marker->Replace(" ","")->Replace(".",",")->Replace(",0,",",_,");
	new_marker = new_marker->Replace("M,","M")->Replace("E,","E")->Replace("S,","S")->Replace("A,","A")->Replace("T,","T");
	new_marker = new_marker->Replace(",0",",")->Replace(",0",",")->Replace("M0","M")->Replace("M0","M");
	new_marker = new_marker->Replace("_,","0,");

	if (ClickedItemText->Contains("NOBI")) //вызов окна ИМ, если такого еще нет, или открыть существующее окно
	{
		if (this->timer2->Enabled)
		{
			for (int n=0; n<this->NobiButton->DropDownItems->Count; n++)
			{
				if (this->NobiButton->DropDownItems[n]->Text == ClickedItemToolTipText) 
				{
					nobi[n]->WindowState = FormWindowState::Normal;
					return;
				}
			}
			int res = this->step_splitter(ClickedItemToolTipText, splitlist);
			if (splitlist->Count == 5 && (splitlist[4] == "ESGS" || splitlist[4] == "ESGM" || splitlist[4] == "ESGV"))	
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::IM(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "BA" || splitlist[4] == "VW"))
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::PVR(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "IVL" || splitlist[4] == "ITE"))
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::IVL(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "IBR") )
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::IM(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "REK") )
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::IM(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "RES") )
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::IM(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
			else if (splitlist->Count == 5 && (splitlist[4] == "KOM") )
			{
				this->NobiButton->DropDownItems->Add(ClickedItemToolTipText);
				nobi->Add(gcnew ArtMoney::KOM(this, splitlist[0],splitlist[1],Convert::ToInt32(splitlist[2]),splitlist[3],splitlist[4]));
				nobi[nobi->Count-1]->Show();
			}
		}
	}
	else // добавить маркер в список имиитаций, если такого еще нет
	{
		bool exist = false;
		try
		{
			debugger->Show();
			for (int i_row=0; i_row<debugger->dataGridView1->RowCount-1; i_row++)
			{
				 try
				 {
					 String^ c_awe = debugger->dataGridView1[0,i_row]->Value->ToString();
					 String^ c_marker = debugger->dataGridView1[1,i_row]->Value->ToString();
					 if (c_awe == new_awe && c_marker == new_marker) exist = true;
				 }
				 catch (Exception^ e) 
				 {
					DWORD err = GetLastError( );
					this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
				 }
			}
			if (!exist)
			{
				 try
				 {
					 int nrow = debugger->dataGridView1->Rows->Count;
					 if (nrow>0) 
					 {
						 debugger->dataGridView1->Rows->Add();
						 debugger->dataGridView1->Rows[nrow-1]->Cells[0]->Value = new_awe;
						 debugger->dataGridView1->Rows[nrow-1]->Cells[1]->Value = new_marker;
					 }
				 }
				 catch (Exception^ e) 
				 {
					DWORD err = GetLastError( );
					this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
				 }
			}		
		}
		catch (Exception^ e) 
		{
			DWORD err = GetLastError( );
			this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
		};
		
	}

	
}

System::Void Form1::EmulatorButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	if (!this->PlayButton->Enabled && this->StopButton->Enabled)
	{
		 try
		 {
			 if (this->EmulatorButton->BackColor == Color::SkyBlue) 
			 {
				 emulated->WindowState = FormWindowState::Minimized;
				 this->EmulatorButton->BackColor = Color::WhiteSmoke;
			 }
			 else 
			 {
				 emulated->WindowState = FormWindowState::Normal;
				 this->EmulatorButton->BackColor = Color::SkyBlue;
			 }
		 }
		 catch (Exception^ e) 
		 {
			DWORD err = GetLastError( );
			this->toolStripLabel1->Text = e->Message + Convert::ToString((int)err);
			this->EmulatorButton->BackColor = Color::WhiteSmoke;
		 }
	}
}

System::Void Form1::DosExec(char * output)
{
	
}