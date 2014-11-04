object MainForm: TMainForm
  Left = 308
  Top = 124
  Width = 491
  Height = 498
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #1055#1045#1063#1040#1058#1068
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 481
    Height = 145
    Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 77
      Height = 13
      Caption = #1041#1072#1079#1072' '#1076#1072#1085#1085#1099#1093
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 48
      Width = 108
      Height = 13
      Caption = #1061#1088#1072#1085#1077#1085#1080#1077' '#1086#1090#1095#1105#1090#1086#1074
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object SaveBitBtn: TBitBtn
      Left = 8
      Top = 104
      Width = 233
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = SaveBitBtnClick
      Kind = bkAll
    end
    object DefaultBitBtn: TBitBtn
      Left = 248
      Top = 104
      Width = 225
      Height = 25
      Caption = #1055#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = DefaultBitBtnClick
      Kind = bkRetry
    end
    object RepFolderNameEdit: TEdit
      Left = 120
      Top = 48
      Width = 337
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 2
      OnChange = RepFolderNameEditChange
    end
    object Button1: TButton
      Left = 456
      Top = 48
      Width = 17
      Height = 21
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = Button1Click
    end
    object BasePathEdit: TEdit
      Left = 120
      Top = 24
      Width = 337
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 4
      OnChange = BasePathEditChange
    end
    object Button2: TButton
      Left = 456
      Top = 24
      Width = 17
      Height = 21
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      OnClick = Button2Click
    end
    object FileRadioButton: TRadioButton
      Left = 120
      Top = 72
      Width = 113
      Height = 17
      Caption = #1055#1077#1095#1072#1090#1100' '#1074' '#1092#1072#1081#1083
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      OnClick = FileRadioButtonClick
    end
    object PrintRadioButton: TRadioButton
      Left = 248
      Top = 72
      Width = 145
      Height = 17
      Caption = #1055#1077#1095#1072#1090#1100' '#1085#1072' '#1087#1088#1080#1085#1090#1077#1088
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      OnClick = PrintRadioButtonClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 152
    Width = 481
    Height = 313
    Caption = #1055#1077#1095#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object Label6: TLabel
      Left = 8
      Top = 24
      Width = 79
      Height = 13
      Caption = #1054#1088#1075#1072#1085#1080#1079#1072#1094#1080#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 8
      Top = 48
      Width = 45
      Height = 13
      Caption = #1055#1077#1088#1080#1086#1076
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object OrgComboBox: TComboBox
      Left = 120
      Top = 24
      Width = 345
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ItemHeight = 13
      ParentFont = False
      TabOrder = 0
    end
    object PeriodDateTimePicker: TDateTimePicker
      Left = 120
      Top = 48
      Width = 345
      Height = 21
      CalAlignment = dtaLeft
      Date = 39309.3617393634
      Format = 'MM.yyyy'
      Time = 39309.3617393634
      DateFormat = dfShort
      DateMode = dmUpDown
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Kind = dtkDate
      ParseInput = False
      ParentFont = False
      TabOrder = 1
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 72
      Width = 465
      Height = 201
      Caption = #1055#1077#1095#1072#1090#1100' '#1076#1086#1082#1091#1084#1077#1085#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object DocCheckListBox: TCheckListBox
        Left = 8
        Top = 16
        Width = 449
        Height = 177
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ItemHeight = 13
        Items.Strings = (
          #1057#1074#1086#1076#1085#1072#1103'  '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1085#1072#1095#1080#1089#1083#1077#1085#1080#1103' '#1082#1074#1072#1088#1090#1087#1083#1072#1090#1099' '#1080' '#1089#1090#1086#1080#1084#1086#1089#1090#1080' '#1091#1089#1083#1091#1075
          #1042#1077#1076#1086#1084#1086#1089#1090#1100' '#1087#1088#1080#1074#1072#1090#1080#1079#1080#1088#1086#1074#1072#1085#1085#1099#1093' '#1082#1074#1072#1088#1090#1080#1088' '#1074' '#1082#1086#1090#1086#1088#1099#1093' '#1085#1080#1082#1090#1086' '#1085#1077' '#1087#1088#1086#1078#1080#1074#1072#1077#1090
          #1048#1090#1086#1075#1080' '#1087#1086' '#1076#1086#1075#1086#1074#1086#1088#1072#1084' '#1085#1072' '#1088#1077#1089#1090#1088#1091#1082#1090#1091#1088#1080#1079#1072#1094#1080#1102' '#1076#1086#1083#1075#1072
          #1057#1074#1086#1076#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1087#1086#1075#1072#1096#1077#1085#1080#1103' '#1079#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1080' '#1087#1086' '#1088#1077#1089#1090#1088#1091#1082#1090#1091#1088#1080#1079#1072#1094#1080#1080
          
            #1057#1074#1086#1076#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1085#1072#1095#1080#1089#1083#1077#1085#1080#1103' '#1086#1087#1083#1072#1090#1099' '#1080' '#1082#1086#1084#1087#1077#1085#1089#1072#1094#1080#1080' '#1087#1086' '#1076#1086#1084#1072#1084' '#1089' '#1083#1080#1092 +
            #1090#1072#1084#1080
          #1057#1074#1086#1076#1085#1072#1103' '#1086#1073#1086#1088#1086#1090#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1088#1072#1089#1095#1105#1090#1086#1074' '#1087#1086' '#1082#1074#1072#1088#1090#1080#1088#1086#1089#1098#1105#1084#1097#1080#1082#1072#1084
          
            #1054#1073#1086#1088#1086#1090#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1088#1072#1089#1095#1105#1090#1086#1074' '#1089' '#1082#1074#1072#1088#1090#1080#1088#1086#1089#1098#1105#1084#1097#1080#1082#1072#1084#1080' '#1087#1086' '#1082#1074#1072#1088#1090#1087#1083#1072#1090#1077' ' +
            #1080' '#1084#1091#1089#1086#1088#1091
          #1042#1077#1076#1086#1084#1086#1089#1090#1100' '#1086#1087#1083#1072#1090#1099' '#1080' '#1079#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1080' '#1087#1086' '#1091#1083#1080#1094#1072#1084
          #1042#1077#1076#1086#1084#1086#1089#1090#1100' '#1076#1077#1073#1077#1090#1086#1088#1089#1082#1086#1081' '#1079#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1080' ('#1074#1099#1074#1086#1079' '#1058#1041#1054')'
          #1057#1074#1086#1076#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1076#1077#1073#1080#1090#1086#1088#1089#1082#1086#1081' '#1079#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1080' ('#1074#1099#1074#1086#1079' '#1058#1041#1054')'
          #1042#1077#1076#1086#1084#1086#1089#1090#1100' '#1076#1077#1073#1080#1090#1086#1088#1089#1082#1086#1081' '#1079#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1080' '#1087#1086' '#1082#1074#1072#1088#1090#1087#1083#1072#1090#1077' '#1080' '#1074#1099#1074#1086#1079#1091' '#1058#1041#1054
          #1057#1074#1086#1076#1085#1072#1103' '#1074#1077#1076#1086#1084#1086#1089#1090#1100' '#1085#1072#1095#1080#1089#1083#1077#1085#1080#1103' '#1087#1086' '#1090#1072#1088#1080#1092#1072#1084' ('#1082#1074#1072#1088#1090#1087#1083#1072#1090#1072')')
        ParentFont = False
        TabOrder = 0
      end
    end
    object PrintBitBtn: TBitBtn
      Left = 168
      Top = 280
      Width = 161
      Height = 25
      Caption = #1055#1077#1095#1072#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = PrintBitBtnClick
      Kind = bkAll
    end
  end
  object FindQuery: TQuery
    SQL.Strings = (
      'select * from G_strit')
    Left = 8
    Top = 160
  end
end
