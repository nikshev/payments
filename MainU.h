//---------------------------------------------------------------------------

#ifndef MainUH
#define MainUH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
struct TIniSettings
{
 AnsiString ReportFolder;
 AnsiString PrinterName;
 AnsiString BasePath;
};

class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TBitBtn *SaveBitBtn;
        TBitBtn *DefaultBitBtn;
        TGroupBox *GroupBox2;
        TLabel *Label6;
        TComboBox *OrgComboBox;
        TLabel *Label8;
        TDateTimePicker *PeriodDateTimePicker;
        TGroupBox *GroupBox3;
        TCheckListBox *DocCheckListBox;
        TBitBtn *PrintBitBtn;
        TEdit *RepFolderNameEdit;
        TButton *Button1;
        TQuery *FindQuery;
        TEdit *BasePathEdit;
        TButton *Button2;
        TRadioButton *FileRadioButton;
        TRadioButton *PrintRadioButton;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall RepFolderNameEditChange(TObject *Sender);
        void __fastcall DefaultBitBtnClick(TObject *Sender);
        void __fastcall SaveBitBtnClick(TObject *Sender);
        void __fastcall PrintBitBtnClick(TObject *Sender);
        void __fastcall BasePathEditChange(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FileRadioButtonClick(TObject *Sender);
        void __fastcall PrintRadioButtonClick(TObject *Sender);
private:	// User declarations
        TIniSettings *pIniSettings;
        DEVMODE *pDevMode;
        bool GetIniSettings(TIniSettings *pIniSettings);
        bool SetIniSettings(TIniSettings *pIniSettings);
        bool GetDefaultIniSettings(TIniSettings *pIniSettings);
        void Rep1(); //Сводная  ведомость начисления квартплаты и стоимости услуг
        void Rep2(); //Ведомость приватизированных квартир в которых никто не проживает
        void Rep3(); //Итоги по договорам на реструктуризацию долга
        void Rep4(); //Сводная ведомость погашения задолженности по реструктуризации
        void Rep5(); //Сводная ведомость начисления оплаты и компенсации по домам с лифтами
        void Rep6(); //Сводная оборотная ведомость расчётов по квартиросъёмщикам
        void Rep7(); //Оборотная ведомость расчётов с квартиросъёмщиками по квартплате и мусору
        void Rep8(); //Ведомость оплаты по улицам
        void Rep9(); //Ведомость дебеторской задолженности (вывоз ТБО)
        void Rep10();//Сводная ведомость дебиторской задолженности (вывоз ТБО)
        void Rep11();//Ведомость дебиторской задолженности по квартплате и вывозу ТБО
        void Rep12();//Сводная ведомость начисления по тарифам (квартплата)
        AnsiString WincharToDoschar(AnsiString WinStr); //Функция переводит символы из одной кодировки в другую
        AnsiString DoscharToWinchar(AnsiString DosStr); //Функция переводит символы из одной кодировки в другую
        void CleanProgramFolder(); //Очищает от кусков запроса программную папку
        AnsiString AlignCenter(AnsiString str,int SymCol); //Оцентровка строки
        AnsiString MonthName(int MonthNum); //Возвращает имя месяца по его номеру
        AnsiString FillHorLine(int SymCol); //Заполняет строку горизонтальными черточками
        void PrintRepWin(TStringList *pStringList,int Format,int Orientation,int RepNum); //Печатает документ в Windows кодировке
        void PrintRepDos(TStringList *pStringList,int Format,int Orientation,int RepNum); //Печатает документ в Dos кодировке
        void FillPrinterList(TStringList *pStringList); //Функция выводит список всех подключённых принтеров
        void FillDeviceMode(PRINTER_DEFAULTSA *out,int Format,int Orientation); //Заполняет структуру DEVICEMODE
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
