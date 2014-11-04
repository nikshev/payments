//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainU.h"
#include "FolderNameU.h"
#include "BasePathU.h"
#include <dbtables.hpp>
#include <IniFiles.hpp>
#include <Printers.hpp>
#include <WInspool.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define VertLine '|'
#define HorLine  '-'
#define SpaceSym 0x20;
#define EndLine  "\r\n"

TMainForm *MainForm;
TFolderNameForm *FolderNameForm;
int OldWidth,OldHeight;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
 AnsiString sqlString,tmpStr;
 bool defSet=false;
 //���������� ������� ����� �� ���������
 OldWidth=MainForm->Width;
 OldHeight=MainForm->Height;

 //��������� �������������� ������ �������� � DocCheckListBox
 int Length=0; //��������� ����� ������ 0
 TCanvas *pCanvas=new TCanvas(); //����� �����
 pCanvas->Handle =GetDC(DocCheckListBox->Handle); //��������� ����� ����� DocCheckListBox

 //����� ������������ ������
 for (int i=0; i<DocCheckListBox->Items->Count; i++)
  {
   AnsiString str=DocCheckListBox->Items->Strings[i];
   int TempLength=pCanvas->TextWidth(str);
   if (TempLength>Length)
    Length=TempLength;
  }

  delete pCanvas;

  //���������� ��������� ��� DocCheckListBox
  SendMessage(DocCheckListBox->Handle,LB_SETHORIZONTALEXTENT,Length,0);


  pIniSettings=new TIniSettings;

  //���������� ��������� TIniSettings
  if (!GetIniSettings(pIniSettings))//���� �� ��������� ��������� TIniSettings
   {
    defSet=true;
    GetDefaultIniSettings(pIniSettings); //���� �������� �� ���������
    SetIniSettings(pIniSettings); //���������� � ini ����
   }

  //������� � ������
  //������� ���� � ���� ������
  BasePathEdit->Text=pIniSettings->BasePath;

  //������� ����� ������
  RepFolderNameEdit->Text=pIniSettings->ReportFolder;

  //������� ��� ��������
  if (pIniSettings->PrinterName=="Print to file")
   {
    FileRadioButton->Checked=true;
    PrintRadioButton->Checked=false;
   }
  else
   {
    FileRadioButton->Checked=false;
    PrintRadioButton->Checked=true;
    }

  //��������� ������ �����������
  //��� ����� ������� ���� � ����
  FindQuery->DatabaseName=pIniSettings->BasePath;
  CleanProgramFolder();
  sqlString="select * from Org";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   for (int i=0; i<FindQuery->RecordCount;i++)
    {
     tmpStr=FindQuery->Fields->Fields[1]->AsString;
     tmpStr=DoscharToWinchar(tmpStr);
     OrgComboBox->Items->Add(tmpStr);
    }

  //���� ���� ����������� ��������� �� ���������
  //������ ��������� ������ ��� ������
  if (!defSet) SaveBitBtn->Enabled=false;
  else SaveBitBtn->Enabled=true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
 Application->CreateForm(__classid(TFolderNameForm), &FolderNameForm);
 FolderNameForm->Visible=true;
}

//---------------------------------------------------------------------------
//������� ����������� �� ini ����� ���������
//� ������� �� ����� �������� TIniSettings
bool TMainForm::GetIniSettings(TIniSettings *pIniSettings)
{
 try
  {
   TIniFile *pIniFile=new TIniFile(ExtractFilePath(Application->ExeName)+"settings.ini");
   pIniSettings->ReportFolder=pIniFile->ReadString("Settings","ReportFolder","");
   pIniSettings->PrinterName=pIniFile->ReadString("Settings","PrinterName","");
   pIniSettings->BasePath=pIniFile->ReadString("Settings","BasePath","");
   delete pIniFile;
  }
  catch (Exception &exception)
  {
   MessageDlg("������ ������ ��������� ���������!",mtError,TMsgDlgButtons() << mbOK, 0);
   return false;
  }
  if ((pIniSettings->ReportFolder!="") &&
      (pIniSettings->PrinterName!="") &&
      (pIniSettings->BasePath!=""))
   return true;
  else
   return false;
}

//---------------------------------------------------------------------------
//������� ���������� � ini ����� ���������
//�������� ������� �������� TIniSettings
bool TMainForm::SetIniSettings(TIniSettings *pIniSettings)
{
 try
  {
   TIniFile *pIniFile=new TIniFile(ExtractFilePath(Application->ExeName)+"settings.ini");
   pIniFile->WriteString("Settings","ReportFolder",pIniSettings->ReportFolder);
   pIniFile->WriteString("Settings","PrinterName",pIniSettings->PrinterName);
   pIniFile->WriteString("Settings","BasePath",pIniSettings->BasePath);
   delete pIniFile;
  }
  catch (Exception &exception)
  {
   MessageDlg("������ ������ ��������� ���������!",mtError,TMsgDlgButtons() << mbOK, 0);
   return false;
  }
 return true;
}

//---------------------------------------------------------------------------
//������� ��������� ��������� TIniSettings ����������� �� ���������
bool TMainForm::GetDefaultIniSettings(TIniSettings *pIniSettings)
{
 pIniSettings->ReportFolder=ExtractFilePath(Application->ExeName)+"Reports\\";
 pIniSettings->BasePath=ExtractFilePath(Application->ExeName)+ "Base\\";
 pIniSettings->PrinterName="Print to file";
 return true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RepFolderNameEditChange(TObject *Sender)
{
 if (RepFolderNameEdit->Text!=pIniSettings->ReportFolder)
  if ((RepFolderNameEdit->Text!="")&& (RepFolderNameEdit->Text!=" "))
   {
    pIniSettings->ReportFolder=RepFolderNameEdit->Text;
    SaveBitBtn->Enabled=true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DefaultBitBtnClick(TObject *Sender)
{
 GetDefaultIniSettings(pIniSettings); //���� �������� �� ���������

 //������� � ������
 //������� ���� ���� ������
 BasePathEdit->Text=pIniSettings->BasePath;  

 //������� ����� �������
 RepFolderNameEdit->Text=pIniSettings->ReportFolder;

  //������� ��� ��������
  if (pIniSettings->PrinterName=="Print to file")
   {
    FileRadioButton->Checked=true;
    PrintRadioButton->Checked=false;
   }
  else
   {
    FileRadioButton->Checked=false;
    PrintRadioButton->Checked=true;
    }
  SaveBitBtn->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveBitBtnClick(TObject *Sender)
{
 SetIniSettings(pIniSettings);
 SaveBitBtn->Enabled=false;
}

//---------------------------------------------------------------------------
//�������  ��������� ���������� ���������� � ��������� �����
//������ ������ - �4
//����������-���������
void TMainForm::Rep1()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ���������";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="���������� ���������� � ��������� ������������ �����";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //�������� ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="����������� ������";
   tmpStr=AlignCenter(tmpStr1,33);
   tmpStr+=VertLine;
   tmpStr1="4";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="5";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="23";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="32";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,26);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,1);
   PrintRepDos(pStringList,0,1,1);
}

//---------------------------------------------------------------------------
//��������� ����������������� ������� � ������� ����� �� ���������
void TMainForm::Rep2()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="���������";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="����������������� ������� � ������� ����� �� ���������";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //�������� ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="�����.";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="��������-";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="������������";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
    tmpStr1="�������������";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,2);
   PrintRepDos(pStringList,0,1,2);
}

//---------------------------------------------------------------------------
//����� �� ��������� �� ���������������� �����
void TMainForm::Rep3()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="����� �� ��������� �� ���������������� �����";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //�������� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,56);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="� � � � � � � � � � ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="��������-";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(56);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="���";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="� �����";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="��.����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="���.����";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,3);
   PrintRepDos(pStringList,0,1,3);
}

//---------------------------------------------------------------------------
//������� ��������� ��������� ������������� �� ����������������
void TMainForm::Rep4()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ��������� ��������� ������������ �� ����������������";
 tmpStr=AlignCenter(tmpStr,114);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,114);
 pStringList->Add(tmpStr);
 //������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,114);
   pStringList->Add(tmpStr);
   //�������� ������
   tmpStr=FillHorLine(114);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1="�";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="����� ���������";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="� �.�. ����������";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="� �.�. ������������";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(90);
   tmpStr+=VertLine;
   tmpStr1="���� ��";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="����� �����";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="����� �����";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="����� �����";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="���������";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(114);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,4);
   PrintRepDos(pStringList,0,1,4);
}

//---------------------------------------------------------------------------
//������� ��������� ���������� ������ � ����������� �� ����� � �������
void TMainForm::Rep5()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ���������";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="����������, ������ � ����������� �� ����� � �������";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //�������� ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,106);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr=FillHorLine(106);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="���������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="��������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="� �.�. ��������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�����������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="���";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(96);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="� �.�. �� ��";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="� �.�. �� ��";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="� �.�. �� ��";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="� �.�. �� ��";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="��� � ����.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="��� � ����.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="��� � ����.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="��� � ����.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //�������� ������
   tmpStr=FillHorLine(106);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,5);
   PrintRepDos(pStringList,0,1,5);
}

//---------------------------------------------------------------------------
//������� ��������� ��������� �������� �� �����������������
void TMainForm::Rep6()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ��������� ��������� ���������";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="���ר��� �� ���������ڨ������";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //�������� ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="�������� ������";
   tmpStr+=AlignCenter(tmpStr1,31);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,20);
   tmpStr+=VertLine;
   tmpStr1="��������";
   tmpStr+=AlignCenter(tmpStr1,26);
   tmpStr+=VertLine;
   tmpStr1="��������� ������";
   tmpStr+=AlignCenter(tmpStr1,31);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="� ����";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(31);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,20);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(26);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(31);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,15);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,20);
   tmpStr+=VertLine;
   tmpStr1="��.��������";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="� �.�. ����";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,15);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,6);
   PrintRepDos(pStringList,0,1,6);
}

//---------------------------------------------------------------------------
//��������� ��������� �������� � ������������������ �� ���������� � ������
void TMainForm::Rep7()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="��������� ���������";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="���ר��� � ���������ڨ������� �� ���������� � ������";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //�������� ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,66);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr=FillHorLine(66);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1="����������";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="���������";
   tmpStr+=AlignCenter(tmpStr1,19);
   tmpStr+=VertLine;
   tmpStr1="��������";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="����������";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="������.";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(19);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1="�";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="���";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�������.";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�.";
   tmpStr+=AlignCenter(tmpStr1,4);
   tmpStr+=VertLine;
   tmpStr1="�.";
   tmpStr+=AlignCenter(tmpStr1,3);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1="���������ڨ�����";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="������.";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�.";
   tmpStr+=AlignCenter(tmpStr1,4);
   tmpStr+=VertLine;
   tmpStr1="�.";
   tmpStr+=AlignCenter(tmpStr1,3);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(66);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,7);
   PrintRepDos(pStringList,0,0,7);
}

//---------------------------------------------------------------------------
//��������� ������ � ������������� �� ������
void TMainForm::Rep8()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="��������� ������ � ������������� �� ������";
 tmpStr=AlignCenter(tmpStr,105);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,105);
 pStringList->Add(tmpStr);
//������ ������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,105);
   pStringList->Add(tmpStr);
   //�������� ������ ������
   tmpStr=FillHorLine(105);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="�����.";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="������";
   tmpStr+=AlignCenter(tmpStr1,17);
   tmpStr+=VertLine;
   tmpStr1="��������-";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="������������";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,17);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="�������������";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(105);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,8);
   PrintRepDos(pStringList,0,1,8);
}

//---------------------------------------------------------------------------
//��������� ����������� ������������� (����� ���)
void TMainForm::Rep9()
{
  AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="��������� ����������� ������������� (����� ���)";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
//������ ������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,150);
   pStringList->Add(tmpStr);
   //�������� ������ ������
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="1 �����";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 2 �� 3 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 4 �� 6 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 7 �� 12 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="����� ���� �� 3-� �.";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="����� 3-� ���";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="���";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(144);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
  //������� ������
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,9);
   PrintRepDos(pStringList,0,0,9);
}

//---------------------------------------------------------------------------
//������� ��������� ����������� ������������� (����� ���)
void TMainForm::Rep10()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ���������";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="����������� ������������� (����� ���)";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
//�������� ������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,75);
   pStringList->Add(tmpStr);
   //����� ������ ������
   tmpStr=FillHorLine(75);
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="�����";
   tmpStr=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="���";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�-��";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�-��";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="����";
   tmpStr=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="�������������";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="�������������";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(75);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,10);
   PrintRepDos(pStringList,0,0,10);
}

//---------------------------------------------------------------------------
//��������� ����������� ������������� �� ���������� � ������ ���
void TMainForm::Rep11()
{
  AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="��������� ����������� �������������";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
//������ ������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,150);
   pStringList->Add(tmpStr);
   //�������� ������ ������
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   //����� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="1 �����";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 2 �� 3 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 4 �� 6 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="�� 7 �� 12 �������";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="����� ���� �� 3-� �.";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="����� 3-� ���";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="���";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(144);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="� �/��";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
  //������� ������
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,11);
   PrintRepDos(pStringList,0,0,11);
}

//---------------------------------------------------------------------------
//������� ��������� ���������� �� ������� (����������)
void TMainForm::Rep12()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //������ ������ ������
 tmpStr="������� ���������";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr="���������� �� ������� (����������)";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
 //������ ������ ������
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="�� ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" �.";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
//�������� ������ ������
  sqlString="select * from Org where NAME_GEP='";
  sqlString+=WincharToDoschar(OrgComboBox->Text);
  sqlString+="'";
  FindQuery->Close();
  FindQuery->SQL->Clear();
  FindQuery->SQL->Add(sqlString);
  FindQuery->Open();
  if (FindQuery->RecordCount>0)
   {
     tmpStr1=FindQuery->Fields->Fields[2]->AsString;
     tmpStr="�.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,84);
   pStringList->Add(tmpStr);
   //����� ������ ������
   tmpStr=FillHorLine(84);
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1="���";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="���.����.";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="���.����";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="���������";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="���-��";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������ ������
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="���.��";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�����";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="�������";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //������� ������
   tmpStr=FillHorLine(84);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,12);
   PrintRepDos(pStringList,0,0,12);
}

//---------------------------------------------------------------------------
//������� ��������� ������� �� ����� ��������� � ������
AnsiString TMainForm::WincharToDoschar(AnsiString WinStr)
{
 AnsiString DosStr;
 int lengthWinStr;
 unsigned char tempChar;

 lengthWinStr=WinStr.Length();
 DosStr="";
 for (int i=1; i<lengthWinStr+1;i++)
  {
   tempChar=WinStr[i];
   if (tempChar == 191) tempChar = 245;
   else if (tempChar == 175)  tempChar = 244;
   else if (tempChar == 186)  tempChar = 243;
   else if (tempChar == 170)  tempChar = 242;
   else if (tempChar == 179)  tempChar = 247;
   else if (tempChar == 178)  tempChar = 246;
   else if ((tempChar>= 192 ) && (tempChar<= 239 )) tempChar = tempChar - 64;
   else if (tempChar>= 240 ) tempChar = tempChar - 16;
   else if (tempChar == 184) tempChar= 240;
   else if (tempChar == 185) tempChar= 252;
   else if (tempChar == 168) tempChar= 241;
   DosStr=DosStr+(char)tempChar;
  }

 return DosStr;

}

//---------------------------------------------------------------------------
//������� ��������� ������� �� ����� ��������� � ������
AnsiString TMainForm::DoscharToWinchar(AnsiString DosStr)
{
 AnsiString WinStr;
 int lengthDosStr;
 unsigned char tempChar;

 lengthDosStr=DosStr.Length();
 WinStr="";

 for (int i=1; i<lengthDosStr+1;i++)
  {
   tempChar=DosStr[i];
   if (tempChar == 245)  tempChar = 191;
   else if (tempChar == 244) tempChar = 175;
   else if (tempChar == 243) tempChar = 186;
   else if (tempChar == 242) tempChar = 170;
   else if (tempChar == 247) tempChar = 179;
   else if (tempChar == 246) tempChar = 178;
   else if ((tempChar >= 128 ) && (tempChar <= 175 )) tempChar = tempChar + 64;
   else if ((tempChar >= 224 ) && (tempChar <= 239 )) tempChar = tempChar + 16;
   else if (tempChar == 240) tempChar = 184;
   else if (tempChar == 241) tempChar = 168;
   else if (tempChar == 252) tempChar = 185;
   WinStr+=(char)tempChar;
  }
 return WinStr;
}

//---------------------------------------------------------------------------
//������� �� ������ ������� ����������� �����
void TMainForm::CleanProgramFolder()
{
 TSearchRec sr;
 int Attrib=0;
 AnsiString tmpStr;

 Attrib |= faReadOnly;
 Attrib |= faHidden;
 Attrib |= faSysFile;
 Attrib |= faVolumeID;
 Attrib |= faDirectory;
 Attrib |= faArchive;
 Attrib |= faAnyFile;

 tmpStr=ExtractFilePath(Application->ExeName)+"*.dbf";
 if (FindFirst(tmpStr, Attrib, sr) == 0)

  {
    do
    {
      if ((sr.Attr & Attrib) == sr.Attr)
       DeleteFile (sr.Name);
     } while (FindNext(sr) == 0);
    FindClose(sr);
  }
}

void __fastcall TMainForm::PrintBitBtnClick(TObject *Sender)
{
 //��������� ��� �������� ���������� ���� �� �����������
 GroupBox1->Enabled=false;
 GroupBox2->Enabled=false;
 if ((OrgComboBox->Text!="") && (OrgComboBox->Text!=" "))
  {
   if (DocCheckListBox->Checked[0]) Rep1();
   if (DocCheckListBox->Checked[1]) Rep2();
   if (DocCheckListBox->Checked[2]) Rep3();
   if (DocCheckListBox->Checked[3]) Rep4();
   if (DocCheckListBox->Checked[4]) Rep5();
   if (DocCheckListBox->Checked[5]) Rep6();
   if (DocCheckListBox->Checked[6]) Rep7();
   if (DocCheckListBox->Checked[7]) Rep8();
   if (DocCheckListBox->Checked[8]) Rep9();
   if (DocCheckListBox->Checked[9]) Rep10();
   if (DocCheckListBox->Checked[10]) Rep11();
   if (DocCheckListBox->Checked[11]) Rep12();
   MessageDlg("������ ������(��) ���������!",mtInformation,TMsgDlgButtons() << mbOK, 0);
  }
  else
   MessageDlg("������! �������� �����������!",mtError,TMsgDlgButtons() << mbOK, 0);

 //�������� ��� �������� ���������� ����� ������
 GroupBox1->Enabled=true;
 GroupBox2->Enabled=true;
}

//---------------------------------------------------------------------------
//���������� ������
AnsiString TMainForm::AlignCenter(AnsiString str,int SymCol)
{
 AnsiString GotovStr,tmpStr;
 float tmp,tmp1,tmp2;
 bool fnum,snum;
 int DoStroki,PosleStroki;
 fnum=false;
 snum=false;
 GotovStr="";
 tmp=SymCol/2;
 tmp1=str.Length()/2;
 tmp2=tmp+tmp1;
 tmp2-=str.Length();
 DoStroki=(int)tmp2;
 PosleStroki=(int)(tmp-tmp1);

 for (int i=0;i<DoStroki;i++) GotovStr+=" "; //��������� ������� ����� �������
 GotovStr+=str;
 for (int i=0;i<PosleStroki;i++) GotovStr+=" "; //��������� ������� ����� ������

 tmpStr="";
 for (int i=1;i<SymCol;i++)
  tmpStr+=GotovStr[i];

 return tmpStr;
}

//---------------------------------------------------------------------------
//���������� ��� ������ �� ��� ������
AnsiString TMainForm::MonthName(int MonthNum)
{
 AnsiString tmpStr;
 switch (MonthNum)
 {
  case 1: tmpStr="������";
          break;
  case 2: tmpStr="�������";
          break;
  case 3: tmpStr="����";
          break;
  case 4: tmpStr="������";
          break;
  case 5: tmpStr="���";
          break;
  case 6: tmpStr="����";
          break;
  case 7: tmpStr="����";
          break;
  case 8: tmpStr="������";
          break;
  case 9: tmpStr="��������";
          break;
  case 10: tmpStr="�������";
          break;
  case 11: tmpStr="������";
          break;
  case 12: tmpStr="�������";
          break;

 }

 return tmpStr;
}

//---------------------------------------------------------------------------
//��������� ������ ��������������� ����������
AnsiString TMainForm::FillHorLine(int SymCol)
{
 AnsiString GotovStr;
 for (int i=0;i<SymCol-1;i++) GotovStr+=HorLine;
 return GotovStr;
}

//---------------------------------------------------------------------------
//�������� �������� � Windows ���������
void TMainForm::PrintRepWin(TStringList *pStringList,int Format,int Orientation,int RepNum)
{
 TMemoryStream *Stream=new TMemoryStream();
 HANDLE hPrinter;     //����� ��������
 PRINTER_DEFAULTS pd; //����� ������� � ��������
 DWORD dwBytes;
 DEVMODE *pDevMode;
 DOC_INFO_1 docinfo1;
 AnsiString tmpStr;
 int pageHeightDefault;
 int pageWidthDefault;
 char tmpChr;

 if (pIniSettings->PrinterName=="Print to file")
  {
   for (int i=0;i<pStringList->Count;i++)
   {
    tmpStr=pStringList->Strings[i];
    tmpStr+=EndLine;
    Stream->Write(tmpStr.c_str(),pStringList->Strings[i].Length()+2);
   }
   Stream->SaveToFile(pIniSettings->ReportFolder+"Rep"+IntToStr(RepNum)+".txt");
  }
 else
  {
   TPrinter *pPrinter=Printer();
   pPrinter->Canvas->Font->Name="Courier";
   pPrinter->Canvas->Font->Size=16;
   pPrinter->Canvas->Font->Height=16;
   if (Orientation==1)
    pPrinter->Orientation=poLandscape;
   else
    pPrinter->Orientation=poPortrait;
    pageWidthDefault=pPrinter->PageWidth;
    pageHeightDefault=pPrinter->PageHeight;
   pPrinter->BeginDoc();
   for (int i=0;i<pStringList->Count;i++)
    pPrinter->Canvas->TextOutA(0,0+(i*pPrinter->Canvas->TextHeight(pStringList->Strings[i].c_str())), pStringList->Strings[i].c_str());
   pPrinter->EndDoc();
   ClosePrinter(Handle);
 }
 delete Stream;
}

//---------------------------------------------------------------------------
//�������� �������� � Dos ���������
void TMainForm::PrintRepDos(TStringList *pStringList,int Format,int Orientation,int RepNum)
{
 AnsiString tmpStr;
 TMemoryStream *Stream=new TMemoryStream();
 for (int i=0;i<pStringList->Count;i++)
  {
   tmpStr=WincharToDoschar(pStringList->Strings[i]);
   tmpStr+=EndLine;
   Stream->Write(tmpStr.c_str(),tmpStr.Length()+2);
   }
   Stream->SaveToFile(pIniSettings->ReportFolder+"RepDos"+IntToStr(RepNum)+".txt");
  delete Stream;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::BasePathEditChange(TObject *Sender)
{
 if (BasePathEdit->Text!=pIniSettings->BasePath)
  if ((BasePathEdit->Text!="")&& (BasePathEdit->Text!=" "))
   {
    pIniSettings->BasePath=BasePathEdit->Text;
    SaveBitBtn->Enabled=true;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
 Application->CreateForm(__classid(TBasePathForm), &BasePathForm);
 BasePathForm->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FormResize(TObject *Sender)
{
 int width,height;
 int tempwidth,tempheight;
 //����������� ���� �������� � ����������� �� �������
 width=MainForm->Width;
 height=MainForm->Height;
 tempwidth=width-OldWidth;
 tempheight=height-OldHeight;
 //�������� ������� � ������ GroupBox
 GroupBox1->Width+=tempwidth;
 Button2->Left+=tempwidth;
 BasePathEdit->Width+=tempwidth;
 Button1->Left+=tempwidth;
 RepFolderNameEdit->Width+=tempwidth;
 FileRadioButton->Left+=tempwidth;
 PrintRadioButton->Left+=tempwidth;
 DefaultBitBtn->Left+=tempwidth;
 //�������� ������� �� ������ GroupBox
 GroupBox2->Width+=tempwidth;
 GroupBox2->Height+=tempheight;
 OrgComboBox->Width+=tempwidth;
 PeriodDateTimePicker->Width+=tempwidth;
 PrintBitBtn->Width+=tempwidth;
 if (PrintBitBtn->Width<161) PrintBitBtn->Width=161;
 PrintBitBtn->Left=(int)((GroupBox2->Width-PrintBitBtn->Width)/2);
 PrintBitBtn->Top=GroupBox2->Height-33;
 //�������� ������� � ������� GroupBox
 GroupBox3->Width+=tempwidth;
 GroupBox3->Height+=tempheight;
 DocCheckListBox->Width+=tempwidth;
 DocCheckListBox->Height+=tempheight;

 OldWidth=width;
 OldHeight=height;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileRadioButtonClick(TObject *Sender)
{
 if (pIniSettings->PrinterName!="Print to file")
  {
   //������� ����� ��������
   SaveBitBtn->Enabled=true;
   pIniSettings->PrinterName="Print to file";
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PrintRadioButtonClick(TObject *Sender)
{
 if (pIniSettings->PrinterName!="Default printer")
  {
   //������� ����� ��������
   SaveBitBtn->Enabled=true;
   pIniSettings->PrinterName="Default printer";
  }
}
//---------------------------------------------------------------------------

