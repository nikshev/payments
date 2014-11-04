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
        void Rep1(); //�������  ��������� ���������� ���������� � ��������� �����
        void Rep2(); //��������� ����������������� ������� � ������� ����� �� ���������
        void Rep3(); //����� �� ��������� �� ���������������� �����
        void Rep4(); //������� ��������� ��������� ������������� �� ����������������
        void Rep5(); //������� ��������� ���������� ������ � ����������� �� ����� � �������
        void Rep6(); //������� ��������� ��������� �������� �� �����������������
        void Rep7(); //��������� ��������� �������� � ������������������ �� ���������� � ������
        void Rep8(); //��������� ������ �� ������
        void Rep9(); //��������� ����������� ������������� (����� ���)
        void Rep10();//������� ��������� ����������� ������������� (����� ���)
        void Rep11();//��������� ����������� ������������� �� ���������� � ������ ���
        void Rep12();//������� ��������� ���������� �� ������� (����������)
        AnsiString WincharToDoschar(AnsiString WinStr); //������� ��������� ������� �� ����� ��������� � ������
        AnsiString DoscharToWinchar(AnsiString DosStr); //������� ��������� ������� �� ����� ��������� � ������
        void CleanProgramFolder(); //������� �� ������ ������� ����������� �����
        AnsiString AlignCenter(AnsiString str,int SymCol); //���������� ������
        AnsiString MonthName(int MonthNum); //���������� ��� ������ �� ��� ������
        AnsiString FillHorLine(int SymCol); //��������� ������ ��������������� ����������
        void PrintRepWin(TStringList *pStringList,int Format,int Orientation,int RepNum); //�������� �������� � Windows ���������
        void PrintRepDos(TStringList *pStringList,int Format,int Orientation,int RepNum); //�������� �������� � Dos ���������
        void FillPrinterList(TStringList *pStringList); //������� ������� ������ ���� ������������ ���������
        void FillDeviceMode(PRINTER_DEFAULTSA *out,int Format,int Orientation); //��������� ��������� DEVICEMODE
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
