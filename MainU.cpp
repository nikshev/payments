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
 //Запоминаем размеры формы по умолчанию
 OldWidth=MainForm->Width;
 OldHeight=MainForm->Height;

 //Добавляем горизонтальную полосу прорутки в DocCheckListBox
 int Length=0; //Начальная длина строки 0
 TCanvas *pCanvas=new TCanvas(); //Новая канва
 pCanvas->Handle =GetDC(DocCheckListBox->Handle); //Назначаем канве хэндл DocCheckListBox

 //Поиск максимальной строки
 for (int i=0; i<DocCheckListBox->Items->Count; i++)
  {
   AnsiString str=DocCheckListBox->Items->Strings[i];
   int TempLength=pCanvas->TextWidth(str);
   if (TempLength>Length)
    Length=TempLength;
  }

  delete pCanvas;

  //Отправляем сообщение для DocCheckListBox
  SendMessage(DocCheckListBox->Handle,LB_SETHORIZONTALEXTENT,Length,0);


  pIniSettings=new TIniSettings;

  //Заполнение структуры TIniSettings
  if (!GetIniSettings(pIniSettings))//Если не заполнена структура TIniSettings
   {
    defSet=true;
    GetDefaultIniSettings(pIniSettings); //Берём значения по умолчанию
    SetIniSettings(pIniSettings); //Записываем в ini файл
   }

  //Заносим в списки
  //Заносим путь к базе данных
  BasePathEdit->Text=pIniSettings->BasePath;

  //Заносим папку отчёто
  RepFolderNameEdit->Text=pIniSettings->ReportFolder;

  //Заносим имя принтера
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

  //Заполняем список организаций
  //Для этого заносим путь к базе
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

  //Если были установлены установки по умолчанию
  //Значит размыкаем кнопку для записи
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
//Функция вытаскивает из ini файла настройки
//и передаёт их через струтуру TIniSettings
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
   MessageDlg("Ошибка чтения установок программы!",mtError,TMsgDlgButtons() << mbOK, 0);
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
//Функция записывает в ini файла настройки
//аргумент функции струтура TIniSettings
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
   MessageDlg("Ошибка чтения установок программы!",mtError,TMsgDlgButtons() << mbOK, 0);
   return false;
  }
 return true;
}

//---------------------------------------------------------------------------
//Функция заполняет структуру TIniSettings установками по умолчанию
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
 GetDefaultIniSettings(pIniSettings); //Берём значения по умолчанию

 //Заносим в списки
 //Заносим путь базы данных
 BasePathEdit->Text=pIniSettings->BasePath;  

 //Заносим папку отчётов
 RepFolderNameEdit->Text=pIniSettings->ReportFolder;

  //Заносим имя принтера
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
//Сводная  ведомость начисления квартплаты и стоимости услуг
//Формат бумаги - А4
//Ориентация-альбомная
void TMainForm::Rep1()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="НАЧИСЛЕНИЕ КВАРТПЛАТЫ И СТОИМОСТИ КОММУНАЛЬНЫХ УСЛУГ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Четвёртая строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="КОМУНАЛЬНЫЕ УСЛУГИ";
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
   tmpStr1="ИТОГО";
   tmpStr+=AlignCenter(tmpStr1,26);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,1);
   PrintRepDos(pStringList,0,1,1);
}

//---------------------------------------------------------------------------
//Ведомость приватизированных квартир в которых никто не проживает
void TMainForm::Rep2()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="ПРИВАТИЗИРОВАННЫХ КВАРТИР В КОТОРЫХ НИКТО НЕ ПРОЖИВАЕТ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Четвёртая строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ШИФР";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="АДРЕС";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="КОЛИЧ.";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ОБЩАЯ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="ЗАДОЛЖЕН-";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="ПРОСРОЧЕННАЯ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Сьдьмая строка
   tmpStr1="ДОМА";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="КВАРТИР";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ПЛОЩАДЬ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="НОСТЬ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
    tmpStr1="ЗАДОЛЖЕННОСТЬ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,2);
   PrintRepDos(pStringList,0,1,2);
}

//---------------------------------------------------------------------------
//Итоги по договорам на реструктуризацию долга
void TMainForm::Rep3()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ИТОГИ ПО ДОГОВОРАМ НА РЕСТРУКТУРИЗАЦИЮ ДОЛГА";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Третья строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //Четвёртая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="МЕСЯЧ";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="ОБОРОТЫ";
   tmpStr+=AlignCenter(tmpStr1,56);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="П Р И М Е Ч А Н И Е ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="ДОГОВОРО-";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="ДОЛГА";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(56);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,32);
   tmpStr+=VertLine;
   tmpStr1="РОВ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="В ДОГОВ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ПОГАШ";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="ВХ.ДОЛГ";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="НАЧИСЛ";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ИСХ.ДОЛГ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,3);
   PrintRepDos(pStringList,0,1,3);
}

//---------------------------------------------------------------------------
//Сводная ведомость погашения задолженности по реструктуризации
void TMainForm::Rep4()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ ПОГАШЕНИЯ ЗАДОЛЖЕНОСТИ ПО РЕСТРУКТУРИЗАЦИИ";
 tmpStr=AlignCenter(tmpStr,114);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,114);
 pStringList->Add(tmpStr);
 //Третья строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,114);
   pStringList->Add(tmpStr);
   //Четвёртая строка
   tmpStr=FillHorLine(114);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1="№";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="ВСЕГО ДОГОВОРОВ";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. ПОГАШЕННЫЕ";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. РАСТОРГНУТЫЕ";
   tmpStr+=AlignCenter(tmpStr1,30);
   tmpStr+=VertLine;
   tmpStr1="ОСТАТОК";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(90);
   tmpStr+=VertLine;
   tmpStr1="ДОЛГ НА";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1="ЖЕКА";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="СУММА ДОЛГА";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="СУММА ДОЛГА";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="СУММА ДОЛГА";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="ПОГАШЕНИЕ";
   tmpStr+=AlignCenter(tmpStr1,18);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr=FillHorLine(114);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,4);
   PrintRepDos(pStringList,0,1,4);
}

//---------------------------------------------------------------------------
//Сводная ведомость начисления оплаты и компенсации по домам с лифтами
void TMainForm::Rep5()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="НАЧИСЛЕНИЙ, ОПЛАТЫ И КОМПЕНСАЦИИ ПО ДОМАМ С ЛИФТАМИ";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,106);
 pStringList->Add(tmpStr);
 //Четвёртая строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,106);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr=FillHorLine(106);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="НАЧИСЛЕНО";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАЧЕНО";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. СУБСИДИЯ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="КОМПЕНСАЦИЯ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ЖЭК";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(96);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ВСЕГО";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. ПО ДО";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ВСЕГО";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. ПО ДО";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ВСЕГО";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. ПО ДО";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ВСЕГО";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. ПО ДО";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="МАМ С ЛИФТ.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="МАМ С ЛИФТ.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="МАМ С ЛИФТ.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="МАМ С ЛИФТ.";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Девятаая строка
   tmpStr=FillHorLine(106);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,5);
   PrintRepDos(pStringList,0,1,5);
}

//---------------------------------------------------------------------------
//Сводная оборотная ведомость расчётов по квартиросъёмщикам
void TMainForm::Rep6()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ ОБОРОТНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="РАСЧЁТОВ ПО КВАРТИРОСЪЁМЩИКАМ";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,116);
 pStringList->Add(tmpStr);
 //Четвёртая строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,116);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="ВХОДЯЩЕЕ САЛЬДО";
   tmpStr+=AlignCenter(tmpStr1,31);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,20);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАЧЕНО";
   tmpStr+=AlignCenter(tmpStr1,26);
   tmpStr+=VertLine;
   tmpStr1="ИСХОДЯЩЕЕ САЛЬДО";
   tmpStr+=AlignCenter(tmpStr1,31);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="№ ЖЭКА";
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
   //Седьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="ДЕБЕТ";
   tmpStr+=AlignCenter(tmpStr1,15);
   tmpStr+=VertLine;
   tmpStr1="КРЕДИТ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,20);
   tmpStr+=VertLine;
   tmpStr1="ВС.ОПЛАЧЕНО";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="В Т.Ч. СУБС";
   tmpStr+=AlignCenter(tmpStr1,13);
   tmpStr+=VertLine;
   tmpStr1="ДЕБЕТ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="КРЕДИТ";
   tmpStr+=AlignCenter(tmpStr1,15);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr=FillHorLine(116);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,6);
   PrintRepDos(pStringList,0,1,6);
}

//---------------------------------------------------------------------------
//Оборотная ведомость расчётов с квартиросъёмщиками по квартплате и мусору
void TMainForm::Rep7()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ОБОРОТНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="РАСЧЁТОВ С КВАРТИРОСЪЁМЩИКАМИ ПО КВАРТПЛАТЕ И МУСОРУ";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,65);
 pStringList->Add(tmpStr);
 //Четвёртая строка
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,66);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr=FillHorLine(66);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1="СПРАВОЧНАЯ";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="САЛЬДО";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="НАЧИСЛЕНО";
   tmpStr+=AlignCenter(tmpStr1,19);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАЧЕНО";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ИНФОРМАЦИЯ";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="НАЧАЛО.";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(19);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1="О";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1="КОНЕЦ";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="ВИД";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="ПОСТОЯН.";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="Д.";
   tmpStr+=AlignCenter(tmpStr1,4);
   tmpStr+=VertLine;
   tmpStr1="М.";
   tmpStr+=AlignCenter(tmpStr1,3);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Восьмая строка
   tmpStr1="КВАРТИРОСЪЁМЩИКЕ";
   tmpStr=AlignCenter(tmpStr1,22);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="НАЧИСЛ.";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="РАЗОВЫЕ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="Б.";
   tmpStr+=AlignCenter(tmpStr1,4);
   tmpStr+=VertLine;
   tmpStr1="О.";
   tmpStr+=AlignCenter(tmpStr1,3);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Девятая строка
   tmpStr=FillHorLine(66);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,7);
   PrintRepDos(pStringList,0,0,7);
}

//---------------------------------------------------------------------------
//Ведомость оплаты и задолженности по улицам
void TMainForm::Rep8()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ВЕДОМОСТЬ ОПЛАТЫ И ЗАДОЛЖЕННОСТИ ПО УЛИЦАМ";
 tmpStr=AlignCenter(tmpStr,105);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="ЗА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,105);
 pStringList->Add(tmpStr);
//Третья строка отчёта
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,105);
   pStringList->Add(tmpStr);
   //Четвёртая строка отчёта
   tmpStr=FillHorLine(105);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1="ШИФР";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="АДРЕС";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="КОЛИЧ.";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="ОБЩАЯ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="ОПЛАТА";
   tmpStr+=AlignCenter(tmpStr1,17);
   tmpStr+=VertLine;
   tmpStr1="ЗАДОЛЖЕН-";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="ПРОСРОЧЕННАЯ";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ДОМА";
   tmpStr=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,27);
   tmpStr+=VertLine;
   tmpStr1="КВАРТИР";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="ПЛОЩАДЬ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1=" ";
   tmpStr+=AlignCenter(tmpStr1,17);
   tmpStr+=VertLine;
   tmpStr1="НОСТЬ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   tmpStr1="ЗАДОЛЖЕННОСТЬ";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr=FillHorLine(105);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,1,8);
   PrintRepDos(pStringList,0,1,8);
}

//---------------------------------------------------------------------------
//Ведомость дебеторской задолженности (вывоз ТБО)
void TMainForm::Rep9()
{
  AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ВЕДОМОСТЬ ДЕБИТОРСКОЙ ЗАДОЛЖЕННОСТИ (ВЫВОЗ ТБО)";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="НА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
//Третья строка отчёта
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,150);
   pStringList->Add(tmpStr);
   //Четвёртая строка отчёта
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="1 МЕСЯЦ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 2 ДО 3 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 4 ДО 6 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 7 ДО 12 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="СВЫШЕ ГОДА ДО 3-Х Л.";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="СВЫШЕ 3-Х ЛЕТ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ЖЭК";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(144);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
  //Восьмая строка
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,9);
   PrintRepDos(pStringList,0,0,9);
}

//---------------------------------------------------------------------------
//Сводная ведомость дебиторской задолженности (вывоз ТБО)
void TMainForm::Rep10()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="ДЕБИТОРСКОЙ ЗАДОЛЖЕННОСТИ (ВЫВОЗ ТБО)";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="НА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,75);
 pStringList->Add(tmpStr);
//Четвёртая строка отчёта
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,75);
   pStringList->Add(tmpStr);
   //Пятая строка отчёта
   tmpStr=FillHorLine(75);
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="НОМЕР";
   tmpStr=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="ТИП";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="ОБЩАЯ";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="К-ВО";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="К-ВО";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ЖЭКА";
   tmpStr=AlignCenter(tmpStr1,8);
   tmpStr+=VertLine;
   tmpStr1="СОБСТВЕННОСТИ";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="ПЛОЩАДЬ";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="КВАРТИР";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ЧЕЛОВЕК";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ЗАДОЛЖЕННОСТИ";
   tmpStr+=AlignCenter(tmpStr1,16);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr=FillHorLine(75);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,10);
   PrintRepDos(pStringList,0,0,10);
}

//---------------------------------------------------------------------------
//Ведомость дебиторской задолженности по квартплате и вывозу ТБО
void TMainForm::Rep11()
{
  AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="ВЕДОМОСТЬ ДЕБИТОРСКОЙ ЗАДОЛЖЕННОСТИ";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="НА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,150);
 pStringList->Add(tmpStr);
//Третья строка отчёта
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,150);
   pStringList->Add(tmpStr);
   //Четвёртая строка отчёта
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   //Пятая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="1 МЕСЯЦ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 2 ДО 3 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 4 ДО 6 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="ОТ 7 ДО 12 МЕСЯЦЕВ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="СВЫШЕ ГОДА ДО 3-Х Л.";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   tmpStr1="СВЫШЕ 3-Х ЛЕТ";
   tmpStr+=AlignCenter(tmpStr1,24);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ЖЭК";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr+=FillHorLine(144);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr1=" ";
   tmpStr=AlignCenter(tmpStr1,5);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="К Л/СЧ";
   tmpStr+=AlignCenter(tmpStr1,10);
   tmpStr+=VertLine;
   tmpStr1="СУММА";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
  //Восьмая строка
   tmpStr=FillHorLine(150);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,11);
   PrintRepDos(pStringList,0,0,11);
}

//---------------------------------------------------------------------------
//Сводная ведомость начисления по тарифам (квартплата)
void TMainForm::Rep12()
{
 AnsiString tmpStr,tmpStr1,sqlString;
 TStringList *pStringList=new TStringList();
 pStringList->Clear();
 //Первая строка отчёта
 tmpStr="СВОДНАЯ ВЕДОМОСТЬ";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
 //Вторая строка отчёта
 tmpStr="НАЧИСЛЕНИЙ ПО ТАРИФАМ (КВАРТПЛАТА)";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
 //Третья строка отчёта
 tmpStr1=DateToStr(PeriodDateTimePicker->DateTime);
 tmpStr="НА ";
 tmpStr+=MonthName(StrToInt(tmpStr1.SubString(4,2)));
 tmpStr+=" ";
 tmpStr+=tmpStr1.SubString(7,4);
 tmpStr+=" г.";
 tmpStr=AlignCenter(tmpStr,84);
 pStringList->Add(tmpStr);
//Четвёртая строка отчёта
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
     tmpStr="г.";
     tmpStr+=DoscharToWinchar(tmpStr1);
     tmpStr+="  ";
     tmpStr+=OrgComboBox->Text;
   }
   else tmpStr=" ";
   tmpStr=AlignCenter(tmpStr,84);
   pStringList->Add(tmpStr);
   //Пятая строка отчёта
   tmpStr=FillHorLine(84);
   pStringList->Add(tmpStr);
   //Шестая строка
   tmpStr1="ЖЭК";
   tmpStr=AlignCenter(tmpStr1,7);
   tmpStr+=VertLine;
   tmpStr1="ТАРИФ";
   tmpStr+=AlignCenter(tmpStr1,11);
   tmpStr+=VertLine;
   tmpStr1="ОБЩ.ПЛОЩ.";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="ЖИЛ.ПЛОЩ";
   tmpStr+=AlignCenter(tmpStr1,12);
   tmpStr+=VertLine;
   tmpStr1="НАЧИСЛЕНО";
   tmpStr+=AlignCenter(tmpStr1,14);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="КОЛ-ВО";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Шестая строка
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
   tmpStr1="ЛИЦ.СЧ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ДОМОВ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   tmpStr1="ЖИЛЬЦОВ";
   tmpStr+=AlignCenter(tmpStr1,9);
   tmpStr+=VertLine;
   pStringList->Add(tmpStr);
   //Седьмая строка
   tmpStr=FillHorLine(84);
   pStringList->Add(tmpStr);
   PrintRepWin(pStringList,0,0,12);
   PrintRepDos(pStringList,0,0,12);
}

//---------------------------------------------------------------------------
//Функция переводит символы из одной кодировки в другую
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
//Функция переводит символы из одной кодировки в другую
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
//Очищает от кусков запроса программную папку
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
 //Отключаем все элементы управления пока не распечатаем
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
   MessageDlg("Печать отчёта(ов) завершена!",mtInformation,TMsgDlgButtons() << mbOK, 0);
  }
  else
   MessageDlg("Ошибка! Выберите организацию!",mtError,TMsgDlgButtons() << mbOK, 0);

 //Включаем все элементы управления после печати
 GroupBox1->Enabled=true;
 GroupBox2->Enabled=true;
}

//---------------------------------------------------------------------------
//Оцентровка строки
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

 for (int i=0;i<DoStroki;i++) GotovStr+=" "; //Добавляем пробелы перед строкой
 GotovStr+=str;
 for (int i=0;i<PosleStroki;i++) GotovStr+=" "; //Добавляем пробелы после строки

 tmpStr="";
 for (int i=1;i<SymCol;i++)
  tmpStr+=GotovStr[i];

 return tmpStr;
}

//---------------------------------------------------------------------------
//Возвращает имя месяца по его номеру
AnsiString TMainForm::MonthName(int MonthNum)
{
 AnsiString tmpStr;
 switch (MonthNum)
 {
  case 1: tmpStr="ЯНВАРЬ";
          break;
  case 2: tmpStr="ФЕВРАЛЬ";
          break;
  case 3: tmpStr="МАРТ";
          break;
  case 4: tmpStr="АПРЕЛЬ";
          break;
  case 5: tmpStr="МАЙ";
          break;
  case 6: tmpStr="ИЮНЬ";
          break;
  case 7: tmpStr="ИЮЛЬ";
          break;
  case 8: tmpStr="АВГУСТ";
          break;
  case 9: tmpStr="СЕНТЯБРЬ";
          break;
  case 10: tmpStr="ОКТЯБРЬ";
          break;
  case 11: tmpStr="НОЯБРЬ";
          break;
  case 12: tmpStr="ДЕКАБРЬ";
          break;

 }

 return tmpStr;
}

//---------------------------------------------------------------------------
//Заполняет строку горизонтальными черточками
AnsiString TMainForm::FillHorLine(int SymCol)
{
 AnsiString GotovStr;
 for (int i=0;i<SymCol-1;i++) GotovStr+=HorLine;
 return GotovStr;
}

//---------------------------------------------------------------------------
//Печатает документ в Windows кодировке
void TMainForm::PrintRepWin(TStringList *pStringList,int Format,int Orientation,int RepNum)
{
 TMemoryStream *Stream=new TMemoryStream();
 HANDLE hPrinter;     //Хэндл принтера
 PRINTER_DEFAULTS pd; //Права доступа к принтеру
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
//Печатает документ в Dos кодировке
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
 //Перемещение всех элментов в зависимости от размера
 width=MainForm->Width;
 height=MainForm->Height;
 tempwidth=width-OldWidth;
 tempheight=height-OldHeight;
 //Изменяем размеры в первом GroupBox
 GroupBox1->Width+=tempwidth;
 Button2->Left+=tempwidth;
 BasePathEdit->Width+=tempwidth;
 Button1->Left+=tempwidth;
 RepFolderNameEdit->Width+=tempwidth;
 FileRadioButton->Left+=tempwidth;
 PrintRadioButton->Left+=tempwidth;
 DefaultBitBtn->Left+=tempwidth;
 //Изменяем размеры во втором GroupBox
 GroupBox2->Width+=tempwidth;
 GroupBox2->Height+=tempheight;
 OrgComboBox->Width+=tempwidth;
 PeriodDateTimePicker->Width+=tempwidth;
 PrintBitBtn->Width+=tempwidth;
 if (PrintBitBtn->Width<161) PrintBitBtn->Width=161;
 PrintBitBtn->Left=(int)((GroupBox2->Width-PrintBitBtn->Width)/2);
 PrintBitBtn->Top=GroupBox2->Height-33;
 //Изменяем размеры в тертьем GroupBox
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
   //Заносим новый параметр
   SaveBitBtn->Enabled=true;
   pIniSettings->PrinterName="Print to file";
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PrintRadioButtonClick(TObject *Sender)
{
 if (pIniSettings->PrinterName!="Default printer")
  {
   //Заносим новый параметр
   SaveBitBtn->Enabled=true;
   pIniSettings->PrinterName="Default printer";
  }
}
//---------------------------------------------------------------------------

