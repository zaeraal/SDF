#pragma once

#include "Model.h"
#include "OpenGL.h"
#include "SettingsForm.h"

namespace SDF {

	using namespace System;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Threading;
	using namespace OpenGLForm;
	using namespace ModelController;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			computing_SDF = false;
			MController = new CModel();
			InitializeComponent();
			OpenGL = gcnew COpenGL(this->panel1, MController);

			this->timer1->Enabled = true;
			MController->logInfo("Form Fully Loaded");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
				delete OpenGL;
				delete MController;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		CModel* MController;
		COpenGL^ OpenGL;
		Thread ^ myThread;
		bool computing_SDF;
#pragma region Form Declarations

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Open;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Save;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Save_As;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Exit;
	private: System::Windows::Forms::ToolStripMenuItem^  showToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_about;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::TextBox^  TB_Filename;
	private: System::Windows::Forms::Label^  LBL_Filename;
	private: System::Windows::Forms::TextBox^  TB_SDF_Value;
	private: System::Windows::Forms::Label^  LBL_SDF_Value;
	private: System::Windows::Forms::TextBox^  TB_Face_ID;
	private: System::Windows::Forms::Label^  LBL_Face_ID;
	private: System::Windows::Forms::TextBox^  TB_Total_Triangles;
	private: System::Windows::Forms::Label^  LBL_Total_Faces;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Octree;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_SDF_Hodnoty;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Wireframe;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Default;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Picking_Hodnoty;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::TextBox^  TB_Status;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Normals;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Compute_SDF;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Compute_Triangulation;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Axes;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_None;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Export;
	private: System::Windows::Forms::GroupBox^  groupBox_Selected_Face;
	private: System::Windows::Forms::TextBox^  TB_Total_Vertices;
	private: System::Windows::Forms::Label^  LBL_Total_Vertices;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage_Info;
	private: System::Windows::Forms::GroupBox^  groupBox_SDF;
	private: System::Windows::Forms::TextBox^  TB_Min_SDF;
	private: System::Windows::Forms::TextBox^  TB_Max_SDF;
	private: System::Windows::Forms::Label^  LBL_Min_SDF;
	private: System::Windows::Forms::Label^  LBL_Max_SDF;
	private: System::Windows::Forms::GroupBox^  groupBox_Total;
	private: System::Windows::Forms::TabPage^  tabPage_SDF;
	private: System::Windows::Forms::TabPage^  tabPage_Visual;
	private: System::Windows::Forms::GroupBox^  groupBox_Status;
	private: System::Windows::Forms::ProgressBar^  progressBar;
	private: System::Windows::Forms::GroupBox^  groupBox_GPU_Settings;
	private: System::Windows::Forms::GroupBox^  groupBox_Params;
	private: System::Windows::Forms::Label^  LBL_Rays;
	private: System::Windows::Forms::Label^  LBL_Cone;
	private: System::Windows::Forms::GroupBox^  groupBox_Mode;
	private: System::Windows::Forms::RadioButton^  RB_SDF_Mode_Debug_GPU;
	private: System::Windows::Forms::RadioButton^  RB_SDF_Mode_GPU;
	private: System::Windows::Forms::RadioButton^  RB_SDF_Mode_CPU;
	private: System::Windows::Forms::TextBox^  TB_Cone;
	private: System::Windows::Forms::TextBox^  TB_Rays;
	private: System::Windows::Forms::TextBox^  TB_Work_Items;
	private: System::Windows::Forms::TextBox^  TB_Work_Groups;
	private: System::Windows::Forms::Label^  LBL_Work_Groups;
	private: System::Windows::Forms::Label^  LBL_Work_Items;
	private: System::Windows::Forms::TextBox^  TB_Radius;
	private: System::Windows::Forms::Label^  LBL_Radius;
	private: System::Windows::Forms::CheckBox^  CHB_Reverted_Rays;
	private: System::Windows::Forms::CheckBox^  CHB_Uniform_Distribution;
	private: System::Windows::Forms::GroupBox^  groupBox_Mesh;
	private: System::Windows::Forms::TextBox^  TB_Threshold;
	private: System::Windows::Forms::Label^  LBL_Threshold;
	private: System::Windows::Forms::TextBox^  TB_Depth;
	private: System::Windows::Forms::Label^  LBL_Alpha;
	private: System::Windows::Forms::Label^  LBL_Depth;
	private: System::Windows::Forms::GroupBox^  groupBox_Display_SDF;
	private: System::Windows::Forms::RadioButton^  RB_Display_SDF_on_Vertices;
	private: System::Windows::Forms::RadioButton^  RB_Display_SDF_on_Faces;
	private: System::Windows::Forms::GroupBox^  groupBox_Normalization;
	private: System::Windows::Forms::RadioButton^  RB_Norm2;
	private: System::Windows::Forms::RadioButton^  RB_Norm1;
	private: System::Windows::Forms::RadioButton^  RB_Norm4;
	private: System::Windows::Forms::RadioButton^  RB_Norm3;
	private: System::Windows::Forms::TrackBar^  trackBar_Alpha;
	private: System::Windows::Forms::GroupBox^  groupBox_Octree;
	private: System::Windows::Forms::ToolStripMenuItem^  TSMI_Compute_Octree;


	private: System::ComponentModel::IContainer^  components;


#pragma endregion
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Open = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->TSMI_Save = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Save_As = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->TSMI_Exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_None = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Default = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Wireframe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_SDF_Hodnoty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Picking_Hodnoty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->TSMI_Octree = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Normals = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Axes = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Compute_Octree = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Compute_SDF = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_Compute_Triangulation = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TSMI_about = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_Info = (gcnew System::Windows::Forms::TabPage());
			this->groupBox_Selected_Face = (gcnew System::Windows::Forms::GroupBox());
			this->LBL_Face_ID = (gcnew System::Windows::Forms::Label());
			this->TB_SDF_Value = (gcnew System::Windows::Forms::TextBox());
			this->TB_Face_ID = (gcnew System::Windows::Forms::TextBox());
			this->LBL_SDF_Value = (gcnew System::Windows::Forms::Label());
			this->groupBox_SDF = (gcnew System::Windows::Forms::GroupBox());
			this->TB_Min_SDF = (gcnew System::Windows::Forms::TextBox());
			this->TB_Max_SDF = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Min_SDF = (gcnew System::Windows::Forms::Label());
			this->LBL_Max_SDF = (gcnew System::Windows::Forms::Label());
			this->groupBox_Total = (gcnew System::Windows::Forms::GroupBox());
			this->LBL_Total_Faces = (gcnew System::Windows::Forms::Label());
			this->TB_Total_Triangles = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Total_Vertices = (gcnew System::Windows::Forms::Label());
			this->TB_Total_Vertices = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Filename = (gcnew System::Windows::Forms::Label());
			this->TB_Filename = (gcnew System::Windows::Forms::TextBox());
			this->tabPage_SDF = (gcnew System::Windows::Forms::TabPage());
			this->groupBox_GPU_Settings = (gcnew System::Windows::Forms::GroupBox());
			this->TB_Work_Items = (gcnew System::Windows::Forms::TextBox());
			this->TB_Work_Groups = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Work_Groups = (gcnew System::Windows::Forms::Label());
			this->LBL_Work_Items = (gcnew System::Windows::Forms::Label());
			this->groupBox_Params = (gcnew System::Windows::Forms::GroupBox());
			this->CHB_Uniform_Distribution = (gcnew System::Windows::Forms::CheckBox());
			this->CHB_Reverted_Rays = (gcnew System::Windows::Forms::CheckBox());
			this->TB_Radius = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Radius = (gcnew System::Windows::Forms::Label());
			this->TB_Cone = (gcnew System::Windows::Forms::TextBox());
			this->TB_Rays = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Rays = (gcnew System::Windows::Forms::Label());
			this->LBL_Cone = (gcnew System::Windows::Forms::Label());
			this->groupBox_Mode = (gcnew System::Windows::Forms::GroupBox());
			this->RB_SDF_Mode_Debug_GPU = (gcnew System::Windows::Forms::RadioButton());
			this->RB_SDF_Mode_GPU = (gcnew System::Windows::Forms::RadioButton());
			this->RB_SDF_Mode_CPU = (gcnew System::Windows::Forms::RadioButton());
			this->tabPage_Visual = (gcnew System::Windows::Forms::TabPage());
			this->groupBox_Octree = (gcnew System::Windows::Forms::GroupBox());
			this->LBL_Depth = (gcnew System::Windows::Forms::Label());
			this->TB_Depth = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Threshold = (gcnew System::Windows::Forms::Label());
			this->TB_Threshold = (gcnew System::Windows::Forms::TextBox());
			this->groupBox_Normalization = (gcnew System::Windows::Forms::GroupBox());
			this->RB_Norm4 = (gcnew System::Windows::Forms::RadioButton());
			this->RB_Norm3 = (gcnew System::Windows::Forms::RadioButton());
			this->RB_Norm2 = (gcnew System::Windows::Forms::RadioButton());
			this->RB_Norm1 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox_Mesh = (gcnew System::Windows::Forms::GroupBox());
			this->trackBar_Alpha = (gcnew System::Windows::Forms::TrackBar());
			this->LBL_Alpha = (gcnew System::Windows::Forms::Label());
			this->groupBox_Display_SDF = (gcnew System::Windows::Forms::GroupBox());
			this->RB_Display_SDF_on_Vertices = (gcnew System::Windows::Forms::RadioButton());
			this->RB_Display_SDF_on_Faces = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox_Status = (gcnew System::Windows::Forms::GroupBox());
			this->TB_Status = (gcnew System::Windows::Forms::TextBox());
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->menuStrip1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage_Info->SuspendLayout();
			this->groupBox_Selected_Face->SuspendLayout();
			this->groupBox_SDF->SuspendLayout();
			this->groupBox_Total->SuspendLayout();
			this->tabPage_SDF->SuspendLayout();
			this->groupBox_GPU_Settings->SuspendLayout();
			this->groupBox_Params->SuspendLayout();
			this->groupBox_Mode->SuspendLayout();
			this->tabPage_Visual->SuspendLayout();
			this->groupBox_Octree->SuspendLayout();
			this->groupBox_Normalization->SuspendLayout();
			this->groupBox_Mesh->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar_Alpha))->BeginInit();
			this->groupBox_Display_SDF->SuspendLayout();
			this->groupBox_Status->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->fileToolStripMenuItem, 
				this->showToolStripMenuItem, this->toolsToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(784, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->TSMI_Open, 
				this->toolStripSeparator, this->TSMI_Save, this->TSMI_Save_As, this->TSMI_Export, this->toolStripSeparator2, this->TSMI_Exit});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// TSMI_Open
			// 
			this->TSMI_Open->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"TSMI_Open.Image")));
			this->TSMI_Open->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->TSMI_Open->Name = L"TSMI_Open";
			this->TSMI_Open->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->TSMI_Open->Size = System::Drawing::Size(154, 22);
			this->TSMI_Open->Text = L"&Open";
			this->TSMI_Open->Click += gcnew System::EventHandler(this, &Form1::TSMI_Open_Click);
			// 
			// toolStripSeparator
			// 
			this->toolStripSeparator->Name = L"toolStripSeparator";
			this->toolStripSeparator->Size = System::Drawing::Size(151, 6);
			// 
			// TSMI_Save
			// 
			this->TSMI_Save->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"TSMI_Save.Image")));
			this->TSMI_Save->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->TSMI_Save->Name = L"TSMI_Save";
			this->TSMI_Save->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->TSMI_Save->Size = System::Drawing::Size(154, 22);
			this->TSMI_Save->Text = L"&Save";
			this->TSMI_Save->Click += gcnew System::EventHandler(this, &Form1::TSMI_Save_Click);
			// 
			// TSMI_Save_As
			// 
			this->TSMI_Save_As->Name = L"TSMI_Save_As";
			this->TSMI_Save_As->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::A));
			this->TSMI_Save_As->Size = System::Drawing::Size(154, 22);
			this->TSMI_Save_As->Text = L"Save &as";
			this->TSMI_Save_As->Click += gcnew System::EventHandler(this, &Form1::TSMI_Save_As_Click);
			// 
			// TSMI_Export
			// 
			this->TSMI_Export->Name = L"TSMI_Export";
			this->TSMI_Export->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->TSMI_Export->Size = System::Drawing::Size(154, 22);
			this->TSMI_Export->Text = L"&Export";
			this->TSMI_Export->Click += gcnew System::EventHandler(this, &Form1::TSMI_Export_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(151, 6);
			// 
			// TSMI_Exit
			// 
			this->TSMI_Exit->Name = L"TSMI_Exit";
			this->TSMI_Exit->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X));
			this->TSMI_Exit->Size = System::Drawing::Size(154, 22);
			this->TSMI_Exit->Text = L"E&xit";
			this->TSMI_Exit->Click += gcnew System::EventHandler(this, &Form1::TSMI_Exit_Click);
			// 
			// showToolStripMenuItem
			// 
			this->showToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->TSMI_None, 
				this->TSMI_Default, this->TSMI_Wireframe, this->TSMI_SDF_Hodnoty, this->TSMI_Picking_Hodnoty, this->toolStripSeparator1, this->TSMI_Octree, 
				this->TSMI_Normals, this->TSMI_Axes});
			this->showToolStripMenuItem->Enabled = false;
			this->showToolStripMenuItem->Name = L"showToolStripMenuItem";
			this->showToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->showToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->showToolStripMenuItem->Text = L"Show";
			// 
			// TSMI_None
			// 
			this->TSMI_None->Name = L"TSMI_None";
			this->TSMI_None->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->TSMI_None->Size = System::Drawing::Size(190, 22);
			this->TSMI_None->Text = L"&None";
			this->TSMI_None->Click += gcnew System::EventHandler(this, &Form1::TSMI_None_Click);
			// 
			// TSMI_Default
			// 
			this->TSMI_Default->Name = L"TSMI_Default";
			this->TSMI_Default->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::D));
			this->TSMI_Default->Size = System::Drawing::Size(190, 22);
			this->TSMI_Default->Text = L"&Default";
			this->TSMI_Default->Click += gcnew System::EventHandler(this, &Form1::TSMI_Default_Click);
			// 
			// TSMI_Wireframe
			// 
			this->TSMI_Wireframe->Name = L"TSMI_Wireframe";
			this->TSMI_Wireframe->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->TSMI_Wireframe->Size = System::Drawing::Size(190, 22);
			this->TSMI_Wireframe->Text = L"&Wireframe";
			this->TSMI_Wireframe->Click += gcnew System::EventHandler(this, &Form1::TSMI_Wireframe_Click);
			// 
			// TSMI_SDF_Hodnoty
			// 
			this->TSMI_SDF_Hodnoty->Name = L"TSMI_SDF_Hodnoty";
			this->TSMI_SDF_Hodnoty->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::F));
			this->TSMI_SDF_Hodnoty->Size = System::Drawing::Size(190, 22);
			this->TSMI_SDF_Hodnoty->Text = L"SD&F values";
			this->TSMI_SDF_Hodnoty->Click += gcnew System::EventHandler(this, &Form1::TSMI_SDF_Hodnoty_Click);
			// 
			// TSMI_Picking_Hodnoty
			// 
			this->TSMI_Picking_Hodnoty->Name = L"TSMI_Picking_Hodnoty";
			this->TSMI_Picking_Hodnoty->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::P));
			this->TSMI_Picking_Hodnoty->Size = System::Drawing::Size(190, 22);
			this->TSMI_Picking_Hodnoty->Text = L"&Picking values";
			this->TSMI_Picking_Hodnoty->Click += gcnew System::EventHandler(this, &Form1::TSMI_Picking_Hodnoty_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(187, 6);
			// 
			// TSMI_Octree
			// 
			this->TSMI_Octree->Name = L"TSMI_Octree";
			this->TSMI_Octree->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::O));
			this->TSMI_Octree->Size = System::Drawing::Size(190, 22);
			this->TSMI_Octree->Text = L"&Octree";
			this->TSMI_Octree->Click += gcnew System::EventHandler(this, &Form1::TSMI_Octree_Click);
			// 
			// TSMI_Normals
			// 
			this->TSMI_Normals->Name = L"TSMI_Normals";
			this->TSMI_Normals->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::N));
			this->TSMI_Normals->Size = System::Drawing::Size(190, 22);
			this->TSMI_Normals->Text = L"&Normals";
			this->TSMI_Normals->Click += gcnew System::EventHandler(this, &Form1::TSMI_Normals_Click);
			// 
			// TSMI_Axes
			// 
			this->TSMI_Axes->Name = L"TSMI_Axes";
			this->TSMI_Axes->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::A));
			this->TSMI_Axes->Size = System::Drawing::Size(190, 22);
			this->TSMI_Axes->Text = L"&Axes";
			this->TSMI_Axes->Click += gcnew System::EventHandler(this, &Form1::TSMI_Axes_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->TSMI_Compute_Octree, 
				this->TSMI_Compute_SDF, this->TSMI_Compute_Triangulation});
			this->toolsToolStripMenuItem->Enabled = false;
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"&Tools";
			// 
			// TSMI_Compute_Octree
			// 
			this->TSMI_Compute_Octree->Name = L"TSMI_Compute_Octree";
			this->TSMI_Compute_Octree->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::M));
			this->TSMI_Compute_Octree->Size = System::Drawing::Size(239, 22);
			this->TSMI_Compute_Octree->Text = L"Co&mpute Octree";
			this->TSMI_Compute_Octree->Click += gcnew System::EventHandler(this, &Form1::TSMI_Compute_Octree_Click);
			// 
			// TSMI_Compute_SDF
			// 
			this->TSMI_Compute_SDF->Name = L"TSMI_Compute_SDF";
			this->TSMI_Compute_SDF->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->TSMI_Compute_SDF->Size = System::Drawing::Size(239, 22);
			this->TSMI_Compute_SDF->Text = L"&Compute SDF";
			this->TSMI_Compute_SDF->Click += gcnew System::EventHandler(this, &Form1::TSMI_Compute_SDF_Click);
			// 
			// TSMI_Compute_Triangulation
			// 
			this->TSMI_Compute_Triangulation->Name = L"TSMI_Compute_Triangulation";
			this->TSMI_Compute_Triangulation->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::T));
			this->TSMI_Compute_Triangulation->Size = System::Drawing::Size(239, 22);
			this->TSMI_Compute_Triangulation->Text = L"Compute &Triangulation";
			this->TSMI_Compute_Triangulation->Click += gcnew System::EventHandler(this, &Form1::TSMI_Compute_Triangulation_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->TSMI_about});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// TSMI_about
			// 
			this->TSMI_about->Name = L"TSMI_about";
			this->TSMI_about->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::A));
			this->TSMI_about->Size = System::Drawing::Size(158, 22);
			this->TSMI_about->Text = L"&About...";
			this->TSMI_about->Click += gcnew System::EventHandler(this, &Form1::TSMI_about_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 50;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->panel1->Location = System::Drawing::Point(180, 27);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(604, 535);
			this->panel1->TabIndex = 1;
			this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->tabControl1);
			this->panel2->Controls->Add(this->groupBox_Status);
			this->panel2->Location = System::Drawing::Point(0, 27);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(174, 535);
			this->panel2->TabIndex = 2;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage_Info);
			this->tabControl1->Controls->Add(this->tabPage_SDF);
			this->tabControl1->Controls->Add(this->tabPage_Visual);
			this->tabControl1->Location = System::Drawing::Point(3, 3);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(168, 408);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage_Info
			// 
			this->tabPage_Info->BackColor = System::Drawing::Color::Silver;
			this->tabPage_Info->Controls->Add(this->groupBox_Selected_Face);
			this->tabPage_Info->Controls->Add(this->groupBox_SDF);
			this->tabPage_Info->Controls->Add(this->groupBox_Total);
			this->tabPage_Info->Controls->Add(this->LBL_Filename);
			this->tabPage_Info->Controls->Add(this->TB_Filename);
			this->tabPage_Info->Location = System::Drawing::Point(4, 22);
			this->tabPage_Info->Name = L"tabPage_Info";
			this->tabPage_Info->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_Info->Size = System::Drawing::Size(160, 382);
			this->tabPage_Info->TabIndex = 0;
			this->tabPage_Info->Text = L"Info";
			// 
			// groupBox_Selected_Face
			// 
			this->groupBox_Selected_Face->Controls->Add(this->LBL_Face_ID);
			this->groupBox_Selected_Face->Controls->Add(this->TB_SDF_Value);
			this->groupBox_Selected_Face->Controls->Add(this->TB_Face_ID);
			this->groupBox_Selected_Face->Controls->Add(this->LBL_SDF_Value);
			this->groupBox_Selected_Face->Location = System::Drawing::Point(8, 261);
			this->groupBox_Selected_Face->Name = L"groupBox_Selected_Face";
			this->groupBox_Selected_Face->Size = System::Drawing::Size(148, 103);
			this->groupBox_Selected_Face->TabIndex = 10;
			this->groupBox_Selected_Face->TabStop = false;
			this->groupBox_Selected_Face->Text = L"Selected Face";
			// 
			// LBL_Face_ID
			// 
			this->LBL_Face_ID->AutoSize = true;
			this->LBL_Face_ID->Location = System::Drawing::Point(6, 18);
			this->LBL_Face_ID->Name = L"LBL_Face_ID";
			this->LBL_Face_ID->Size = System::Drawing::Size(18, 13);
			this->LBL_Face_ID->TabIndex = 2;
			this->LBL_Face_ID->Text = L"ID";
			// 
			// TB_SDF_Value
			// 
			this->TB_SDF_Value->Location = System::Drawing::Point(9, 75);
			this->TB_SDF_Value->Name = L"TB_SDF_Value";
			this->TB_SDF_Value->ReadOnly = true;
			this->TB_SDF_Value->Size = System::Drawing::Size(134, 20);
			this->TB_SDF_Value->TabIndex = 5;
			this->TB_SDF_Value->Text = L"0";
			this->TB_SDF_Value->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// TB_Face_ID
			// 
			this->TB_Face_ID->Location = System::Drawing::Point(9, 35);
			this->TB_Face_ID->Name = L"TB_Face_ID";
			this->TB_Face_ID->ReadOnly = true;
			this->TB_Face_ID->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Face_ID->Size = System::Drawing::Size(134, 20);
			this->TB_Face_ID->TabIndex = 3;
			this->TB_Face_ID->Text = L"-1";
			this->TB_Face_ID->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// LBL_SDF_Value
			// 
			this->LBL_SDF_Value->AutoSize = true;
			this->LBL_SDF_Value->Location = System::Drawing::Point(6, 58);
			this->LBL_SDF_Value->Name = L"LBL_SDF_Value";
			this->LBL_SDF_Value->Size = System::Drawing::Size(58, 13);
			this->LBL_SDF_Value->TabIndex = 4;
			this->LBL_SDF_Value->Text = L"SDF Value";
			// 
			// groupBox_SDF
			// 
			this->groupBox_SDF->Controls->Add(this->TB_Min_SDF);
			this->groupBox_SDF->Controls->Add(this->TB_Max_SDF);
			this->groupBox_SDF->Controls->Add(this->LBL_Min_SDF);
			this->groupBox_SDF->Controls->Add(this->LBL_Max_SDF);
			this->groupBox_SDF->Location = System::Drawing::Point(8, 153);
			this->groupBox_SDF->Name = L"groupBox_SDF";
			this->groupBox_SDF->Size = System::Drawing::Size(149, 102);
			this->groupBox_SDF->TabIndex = 12;
			this->groupBox_SDF->TabStop = false;
			this->groupBox_SDF->Text = L"SDF";
			// 
			// TB_Min_SDF
			// 
			this->TB_Min_SDF->Location = System::Drawing::Point(6, 33);
			this->TB_Min_SDF->Name = L"TB_Min_SDF";
			this->TB_Min_SDF->ReadOnly = true;
			this->TB_Min_SDF->Size = System::Drawing::Size(137, 20);
			this->TB_Min_SDF->TabIndex = 11;
			this->TB_Min_SDF->Text = L"0";
			this->TB_Min_SDF->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// TB_Max_SDF
			// 
			this->TB_Max_SDF->Location = System::Drawing::Point(6, 73);
			this->TB_Max_SDF->Name = L"TB_Max_SDF";
			this->TB_Max_SDF->ReadOnly = true;
			this->TB_Max_SDF->Size = System::Drawing::Size(137, 20);
			this->TB_Max_SDF->TabIndex = 13;
			this->TB_Max_SDF->Text = L"0";
			this->TB_Max_SDF->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// LBL_Min_SDF
			// 
			this->LBL_Min_SDF->AutoSize = true;
			this->LBL_Min_SDF->Location = System::Drawing::Point(4, 17);
			this->LBL_Min_SDF->Name = L"LBL_Min_SDF";
			this->LBL_Min_SDF->Size = System::Drawing::Size(24, 13);
			this->LBL_Min_SDF->TabIndex = 10;
			this->LBL_Min_SDF->Text = L"Min";
			// 
			// LBL_Max_SDF
			// 
			this->LBL_Max_SDF->AutoSize = true;
			this->LBL_Max_SDF->Location = System::Drawing::Point(3, 56);
			this->LBL_Max_SDF->Name = L"LBL_Max_SDF";
			this->LBL_Max_SDF->Size = System::Drawing::Size(27, 13);
			this->LBL_Max_SDF->TabIndex = 12;
			this->LBL_Max_SDF->Text = L"Max";
			// 
			// groupBox_Total
			// 
			this->groupBox_Total->Controls->Add(this->LBL_Total_Faces);
			this->groupBox_Total->Controls->Add(this->TB_Total_Triangles);
			this->groupBox_Total->Controls->Add(this->LBL_Total_Vertices);
			this->groupBox_Total->Controls->Add(this->TB_Total_Vertices);
			this->groupBox_Total->Location = System::Drawing::Point(6, 45);
			this->groupBox_Total->Name = L"groupBox_Total";
			this->groupBox_Total->Size = System::Drawing::Size(149, 102);
			this->groupBox_Total->TabIndex = 11;
			this->groupBox_Total->TabStop = false;
			// 
			// LBL_Total_Faces
			// 
			this->LBL_Total_Faces->AutoSize = true;
			this->LBL_Total_Faces->Location = System::Drawing::Point(6, 16);
			this->LBL_Total_Faces->Name = L"LBL_Total_Faces";
			this->LBL_Total_Faces->Size = System::Drawing::Size(63, 13);
			this->LBL_Total_Faces->TabIndex = 6;
			this->LBL_Total_Faces->Text = L"Total Faces";
			// 
			// TB_Total_Triangles
			// 
			this->TB_Total_Triangles->Location = System::Drawing::Point(9, 33);
			this->TB_Total_Triangles->Name = L"TB_Total_Triangles";
			this->TB_Total_Triangles->ReadOnly = true;
			this->TB_Total_Triangles->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Total_Triangles->Size = System::Drawing::Size(134, 20);
			this->TB_Total_Triangles->TabIndex = 7;
			this->TB_Total_Triangles->Text = L"0";
			this->TB_Total_Triangles->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// LBL_Total_Vertices
			// 
			this->LBL_Total_Vertices->AutoSize = true;
			this->LBL_Total_Vertices->Location = System::Drawing::Point(6, 56);
			this->LBL_Total_Vertices->Name = L"LBL_Total_Vertices";
			this->LBL_Total_Vertices->Size = System::Drawing::Size(72, 13);
			this->LBL_Total_Vertices->TabIndex = 8;
			this->LBL_Total_Vertices->Text = L"Total Vertices";
			// 
			// TB_Total_Vertices
			// 
			this->TB_Total_Vertices->Location = System::Drawing::Point(9, 73);
			this->TB_Total_Vertices->Name = L"TB_Total_Vertices";
			this->TB_Total_Vertices->ReadOnly = true;
			this->TB_Total_Vertices->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Total_Vertices->Size = System::Drawing::Size(134, 20);
			this->TB_Total_Vertices->TabIndex = 9;
			this->TB_Total_Vertices->Text = L"0";
			this->TB_Total_Vertices->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// LBL_Filename
			// 
			this->LBL_Filename->AutoSize = true;
			this->LBL_Filename->Location = System::Drawing::Point(3, 3);
			this->LBL_Filename->Name = L"LBL_Filename";
			this->LBL_Filename->Size = System::Drawing::Size(49, 13);
			this->LBL_Filename->TabIndex = 0;
			this->LBL_Filename->Text = L"Filename";
			// 
			// TB_Filename
			// 
			this->TB_Filename->Location = System::Drawing::Point(6, 19);
			this->TB_Filename->Name = L"TB_Filename";
			this->TB_Filename->ReadOnly = true;
			this->TB_Filename->Size = System::Drawing::Size(150, 20);
			this->TB_Filename->TabIndex = 1;
			// 
			// tabPage_SDF
			// 
			this->tabPage_SDF->BackColor = System::Drawing::Color::Silver;
			this->tabPage_SDF->Controls->Add(this->groupBox_GPU_Settings);
			this->tabPage_SDF->Controls->Add(this->groupBox_Params);
			this->tabPage_SDF->Controls->Add(this->groupBox_Mode);
			this->tabPage_SDF->Location = System::Drawing::Point(4, 22);
			this->tabPage_SDF->Name = L"tabPage_SDF";
			this->tabPage_SDF->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_SDF->Size = System::Drawing::Size(160, 382);
			this->tabPage_SDF->TabIndex = 1;
			this->tabPage_SDF->Text = L"SDF";
			// 
			// groupBox_GPU_Settings
			// 
			this->groupBox_GPU_Settings->Controls->Add(this->TB_Work_Items);
			this->groupBox_GPU_Settings->Controls->Add(this->TB_Work_Groups);
			this->groupBox_GPU_Settings->Controls->Add(this->LBL_Work_Groups);
			this->groupBox_GPU_Settings->Controls->Add(this->LBL_Work_Items);
			this->groupBox_GPU_Settings->Location = System::Drawing::Point(8, 264);
			this->groupBox_GPU_Settings->Name = L"groupBox_GPU_Settings";
			this->groupBox_GPU_Settings->Size = System::Drawing::Size(148, 103);
			this->groupBox_GPU_Settings->TabIndex = 15;
			this->groupBox_GPU_Settings->TabStop = false;
			this->groupBox_GPU_Settings->Text = L"GPU Settings";
			// 
			// TB_Work_Items
			// 
			this->TB_Work_Items->Location = System::Drawing::Point(6, 71);
			this->TB_Work_Items->Name = L"TB_Work_Items";
			this->TB_Work_Items->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Work_Items->Size = System::Drawing::Size(136, 20);
			this->TB_Work_Items->TabIndex = 17;
			this->TB_Work_Items->Text = L"0.25";
			this->TB_Work_Items->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Work_Items->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Work_Items_TextChanged);
			// 
			// TB_Work_Groups
			// 
			this->TB_Work_Groups->Location = System::Drawing::Point(6, 32);
			this->TB_Work_Groups->Name = L"TB_Work_Groups";
			this->TB_Work_Groups->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Work_Groups->Size = System::Drawing::Size(136, 20);
			this->TB_Work_Groups->TabIndex = 14;
			this->TB_Work_Groups->Text = L"2.0";
			this->TB_Work_Groups->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Work_Groups->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Work_Groups_TextChanged);
			// 
			// LBL_Work_Groups
			// 
			this->LBL_Work_Groups->AutoSize = true;
			this->LBL_Work_Groups->Location = System::Drawing::Point(4, 16);
			this->LBL_Work_Groups->Name = L"LBL_Work_Groups";
			this->LBL_Work_Groups->Size = System::Drawing::Size(70, 13);
			this->LBL_Work_Groups->TabIndex = 15;
			this->LBL_Work_Groups->Text = L"Work Groups";
			// 
			// LBL_Work_Items
			// 
			this->LBL_Work_Items->AutoSize = true;
			this->LBL_Work_Items->Location = System::Drawing::Point(6, 55);
			this->LBL_Work_Items->Name = L"LBL_Work_Items";
			this->LBL_Work_Items->Size = System::Drawing::Size(61, 13);
			this->LBL_Work_Items->TabIndex = 16;
			this->LBL_Work_Items->Text = L"Work Items";
			// 
			// groupBox_Params
			// 
			this->groupBox_Params->Controls->Add(this->CHB_Uniform_Distribution);
			this->groupBox_Params->Controls->Add(this->CHB_Reverted_Rays);
			this->groupBox_Params->Controls->Add(this->TB_Radius);
			this->groupBox_Params->Controls->Add(this->LBL_Radius);
			this->groupBox_Params->Controls->Add(this->TB_Cone);
			this->groupBox_Params->Controls->Add(this->TB_Rays);
			this->groupBox_Params->Controls->Add(this->LBL_Rays);
			this->groupBox_Params->Controls->Add(this->LBL_Cone);
			this->groupBox_Params->Location = System::Drawing::Point(8, 74);
			this->groupBox_Params->Name = L"groupBox_Params";
			this->groupBox_Params->Size = System::Drawing::Size(149, 184);
			this->groupBox_Params->TabIndex = 17;
			this->groupBox_Params->TabStop = false;
			this->groupBox_Params->Text = L"Algorithm Params";
			// 
			// CHB_Uniform_Distribution
			// 
			this->CHB_Uniform_Distribution->AutoSize = true;
			this->CHB_Uniform_Distribution->Checked = true;
			this->CHB_Uniform_Distribution->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CHB_Uniform_Distribution->Location = System::Drawing::Point(5, 160);
			this->CHB_Uniform_Distribution->Name = L"CHB_Uniform_Distribution";
			this->CHB_Uniform_Distribution->Size = System::Drawing::Size(117, 17);
			this->CHB_Uniform_Distribution->TabIndex = 17;
			this->CHB_Uniform_Distribution->Text = L"Uniform Distribution";
			this->CHB_Uniform_Distribution->UseVisualStyleBackColor = true;
			this->CHB_Uniform_Distribution->CheckedChanged += gcnew System::EventHandler(this, &Form1::CHB_Uniform_Distribution_CheckedChanged);
			// 
			// CHB_Reverted_Rays
			// 
			this->CHB_Reverted_Rays->AutoSize = true;
			this->CHB_Reverted_Rays->Location = System::Drawing::Point(6, 137);
			this->CHB_Reverted_Rays->Name = L"CHB_Reverted_Rays";
			this->CHB_Reverted_Rays->Size = System::Drawing::Size(97, 17);
			this->CHB_Reverted_Rays->TabIndex = 16;
			this->CHB_Reverted_Rays->Text = L"Reverted Rays";
			this->CHB_Reverted_Rays->UseVisualStyleBackColor = true;
			this->CHB_Reverted_Rays->CheckedChanged += gcnew System::EventHandler(this, &Form1::CHB_Reverted_Rays_CheckedChanged);
			// 
			// TB_Radius
			// 
			this->TB_Radius->Location = System::Drawing::Point(6, 111);
			this->TB_Radius->Name = L"TB_Radius";
			this->TB_Radius->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Radius->Size = System::Drawing::Size(136, 20);
			this->TB_Radius->TabIndex = 15;
			this->TB_Radius->Text = L"2";
			this->TB_Radius->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Radius->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Radius_TextChanged);
			// 
			// LBL_Radius
			// 
			this->LBL_Radius->AutoSize = true;
			this->LBL_Radius->Location = System::Drawing::Point(6, 95);
			this->LBL_Radius->Name = L"LBL_Radius";
			this->LBL_Radius->Size = System::Drawing::Size(93, 13);
			this->LBL_Radius->TabIndex = 14;
			this->LBL_Radius->Text = L"Smoothing Radius";
			// 
			// TB_Cone
			// 
			this->TB_Cone->Location = System::Drawing::Point(6, 72);
			this->TB_Cone->Name = L"TB_Cone";
			this->TB_Cone->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Cone->Size = System::Drawing::Size(136, 20);
			this->TB_Cone->TabIndex = 13;
			this->TB_Cone->Text = L"120";
			this->TB_Cone->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Cone->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Cone_TextChanged);
			// 
			// TB_Rays
			// 
			this->TB_Rays->Location = System::Drawing::Point(6, 33);
			this->TB_Rays->Name = L"TB_Rays";
			this->TB_Rays->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Rays->Size = System::Drawing::Size(136, 20);
			this->TB_Rays->TabIndex = 0;
			this->TB_Rays->Text = L"30";
			this->TB_Rays->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Rays->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Rays_TextChanged);
			// 
			// LBL_Rays
			// 
			this->LBL_Rays->AutoSize = true;
			this->LBL_Rays->Location = System::Drawing::Point(4, 17);
			this->LBL_Rays->Name = L"LBL_Rays";
			this->LBL_Rays->Size = System::Drawing::Size(31, 13);
			this->LBL_Rays->TabIndex = 10;
			this->LBL_Rays->Text = L"Rays";
			// 
			// LBL_Cone
			// 
			this->LBL_Cone->AutoSize = true;
			this->LBL_Cone->Location = System::Drawing::Point(6, 56);
			this->LBL_Cone->Name = L"LBL_Cone";
			this->LBL_Cone->Size = System::Drawing::Size(62, 13);
			this->LBL_Cone->TabIndex = 12;
			this->LBL_Cone->Text = L"Cone Angle";
			// 
			// groupBox_Mode
			// 
			this->groupBox_Mode->Controls->Add(this->RB_SDF_Mode_Debug_GPU);
			this->groupBox_Mode->Controls->Add(this->RB_SDF_Mode_GPU);
			this->groupBox_Mode->Controls->Add(this->RB_SDF_Mode_CPU);
			this->groupBox_Mode->Location = System::Drawing::Point(8, 3);
			this->groupBox_Mode->Name = L"groupBox_Mode";
			this->groupBox_Mode->Size = System::Drawing::Size(149, 65);
			this->groupBox_Mode->TabIndex = 16;
			this->groupBox_Mode->TabStop = false;
			this->groupBox_Mode->Text = L"Mode";
			// 
			// RB_SDF_Mode_Debug_GPU
			// 
			this->RB_SDF_Mode_Debug_GPU->AutoSize = true;
			this->RB_SDF_Mode_Debug_GPU->Location = System::Drawing::Point(8, 42);
			this->RB_SDF_Mode_Debug_GPU->Name = L"RB_SDF_Mode_Debug_GPU";
			this->RB_SDF_Mode_Debug_GPU->Size = System::Drawing::Size(89, 17);
			this->RB_SDF_Mode_Debug_GPU->TabIndex = 2;
			this->RB_SDF_Mode_Debug_GPU->Text = L"Debug (GPU)";
			this->RB_SDF_Mode_Debug_GPU->UseVisualStyleBackColor = true;
			this->RB_SDF_Mode_Debug_GPU->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_SDF_Mode_Debug_GPU_CheckedChanged);
			// 
			// RB_SDF_Mode_GPU
			// 
			this->RB_SDF_Mode_GPU->AutoSize = true;
			this->RB_SDF_Mode_GPU->Checked = true;
			this->RB_SDF_Mode_GPU->Location = System::Drawing::Point(61, 19);
			this->RB_SDF_Mode_GPU->Name = L"RB_SDF_Mode_GPU";
			this->RB_SDF_Mode_GPU->Size = System::Drawing::Size(48, 17);
			this->RB_SDF_Mode_GPU->TabIndex = 1;
			this->RB_SDF_Mode_GPU->TabStop = true;
			this->RB_SDF_Mode_GPU->Text = L"GPU";
			this->RB_SDF_Mode_GPU->UseVisualStyleBackColor = true;
			this->RB_SDF_Mode_GPU->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_SDF_Mode_GPU_CheckedChanged);
			// 
			// RB_SDF_Mode_CPU
			// 
			this->RB_SDF_Mode_CPU->AutoSize = true;
			this->RB_SDF_Mode_CPU->Location = System::Drawing::Point(8, 19);
			this->RB_SDF_Mode_CPU->Name = L"RB_SDF_Mode_CPU";
			this->RB_SDF_Mode_CPU->Size = System::Drawing::Size(47, 17);
			this->RB_SDF_Mode_CPU->TabIndex = 0;
			this->RB_SDF_Mode_CPU->Text = L"CPU";
			this->RB_SDF_Mode_CPU->UseVisualStyleBackColor = true;
			this->RB_SDF_Mode_CPU->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_SDF_Mode_CPU_CheckedChanged);
			// 
			// tabPage_Visual
			// 
			this->tabPage_Visual->BackColor = System::Drawing::Color::Silver;
			this->tabPage_Visual->Controls->Add(this->groupBox_Octree);
			this->tabPage_Visual->Controls->Add(this->groupBox_Normalization);
			this->tabPage_Visual->Controls->Add(this->groupBox_Mesh);
			this->tabPage_Visual->Controls->Add(this->groupBox_Display_SDF);
			this->tabPage_Visual->Location = System::Drawing::Point(4, 22);
			this->tabPage_Visual->Name = L"tabPage_Visual";
			this->tabPage_Visual->Size = System::Drawing::Size(160, 382);
			this->tabPage_Visual->TabIndex = 2;
			this->tabPage_Visual->Text = L"Visual";
			// 
			// groupBox_Octree
			// 
			this->groupBox_Octree->Controls->Add(this->LBL_Depth);
			this->groupBox_Octree->Controls->Add(this->TB_Depth);
			this->groupBox_Octree->Controls->Add(this->LBL_Threshold);
			this->groupBox_Octree->Controls->Add(this->TB_Threshold);
			this->groupBox_Octree->Location = System::Drawing::Point(8, 223);
			this->groupBox_Octree->Name = L"groupBox_Octree";
			this->groupBox_Octree->Size = System::Drawing::Size(149, 103);
			this->groupBox_Octree->TabIndex = 21;
			this->groupBox_Octree->TabStop = false;
			this->groupBox_Octree->Text = L"Octree";
			// 
			// LBL_Depth
			// 
			this->LBL_Depth->AutoSize = true;
			this->LBL_Depth->Location = System::Drawing::Point(5, 16);
			this->LBL_Depth->Name = L"LBL_Depth";
			this->LBL_Depth->Size = System::Drawing::Size(36, 13);
			this->LBL_Depth->TabIndex = 12;
			this->LBL_Depth->Text = L"Depth";
			// 
			// TB_Depth
			// 
			this->TB_Depth->Location = System::Drawing::Point(5, 32);
			this->TB_Depth->Name = L"TB_Depth";
			this->TB_Depth->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Depth->Size = System::Drawing::Size(136, 20);
			this->TB_Depth->TabIndex = 13;
			this->TB_Depth->Text = L"8";
			this->TB_Depth->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Depth->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Depth_TextChanged);
			// 
			// LBL_Threshold
			// 
			this->LBL_Threshold->AutoSize = true;
			this->LBL_Threshold->Location = System::Drawing::Point(5, 55);
			this->LBL_Threshold->Name = L"LBL_Threshold";
			this->LBL_Threshold->Size = System::Drawing::Size(54, 13);
			this->LBL_Threshold->TabIndex = 14;
			this->LBL_Threshold->Text = L"Threshold";
			// 
			// TB_Threshold
			// 
			this->TB_Threshold->Location = System::Drawing::Point(5, 71);
			this->TB_Threshold->Name = L"TB_Threshold";
			this->TB_Threshold->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->TB_Threshold->Size = System::Drawing::Size(136, 20);
			this->TB_Threshold->TabIndex = 15;
			this->TB_Threshold->Text = L"4";
			this->TB_Threshold->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->TB_Threshold->TextChanged += gcnew System::EventHandler(this, &Form1::TB_Threshold_TextChanged);
			// 
			// groupBox_Normalization
			// 
			this->groupBox_Normalization->Controls->Add(this->RB_Norm4);
			this->groupBox_Normalization->Controls->Add(this->RB_Norm3);
			this->groupBox_Normalization->Controls->Add(this->RB_Norm2);
			this->groupBox_Normalization->Controls->Add(this->RB_Norm1);
			this->groupBox_Normalization->Location = System::Drawing::Point(8, 55);
			this->groupBox_Normalization->Name = L"groupBox_Normalization";
			this->groupBox_Normalization->Size = System::Drawing::Size(149, 74);
			this->groupBox_Normalization->TabIndex = 21;
			this->groupBox_Normalization->TabStop = false;
			this->groupBox_Normalization->Text = L"Normalization";
			// 
			// RB_Norm4
			// 
			this->RB_Norm4->AutoSize = true;
			this->RB_Norm4->Location = System::Drawing::Point(68, 42);
			this->RB_Norm4->Name = L"RB_Norm4";
			this->RB_Norm4->Size = System::Drawing::Size(71, 17);
			this->RB_Norm4->TabIndex = 3;
			this->RB_Norm4->Text = L"Min - Max";
			this->RB_Norm4->UseVisualStyleBackColor = true;
			this->RB_Norm4->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Norm4_CheckedChanged);
			// 
			// RB_Norm3
			// 
			this->RB_Norm3->AutoSize = true;
			this->RB_Norm3->Checked = true;
			this->RB_Norm3->Location = System::Drawing::Point(8, 42);
			this->RB_Norm3->Name = L"RB_Norm3";
			this->RB_Norm3->Size = System::Drawing::Size(60, 17);
			this->RB_Norm3->TabIndex = 2;
			this->RB_Norm3->TabStop = true;
			this->RB_Norm3->Text = L"0 - Max";
			this->RB_Norm3->UseVisualStyleBackColor = true;
			this->RB_Norm3->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Norm3_CheckedChanged);
			// 
			// RB_Norm2
			// 
			this->RB_Norm2->AutoSize = true;
			this->RB_Norm2->Location = System::Drawing::Point(68, 19);
			this->RB_Norm2->Name = L"RB_Norm2";
			this->RB_Norm2->Size = System::Drawing::Size(57, 17);
			this->RB_Norm2->TabIndex = 1;
			this->RB_Norm2->Text = L"Min - 1";
			this->RB_Norm2->UseVisualStyleBackColor = true;
			this->RB_Norm2->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Norm2_CheckedChanged);
			// 
			// RB_Norm1
			// 
			this->RB_Norm1->AutoSize = true;
			this->RB_Norm1->Location = System::Drawing::Point(8, 19);
			this->RB_Norm1->Name = L"RB_Norm1";
			this->RB_Norm1->Size = System::Drawing::Size(46, 17);
			this->RB_Norm1->TabIndex = 0;
			this->RB_Norm1->Text = L"0 - 1";
			this->RB_Norm1->UseVisualStyleBackColor = true;
			this->RB_Norm1->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Norm1_CheckedChanged);
			// 
			// groupBox_Mesh
			// 
			this->groupBox_Mesh->Controls->Add(this->trackBar_Alpha);
			this->groupBox_Mesh->Controls->Add(this->LBL_Alpha);
			this->groupBox_Mesh->Location = System::Drawing::Point(8, 135);
			this->groupBox_Mesh->Name = L"groupBox_Mesh";
			this->groupBox_Mesh->Size = System::Drawing::Size(149, 82);
			this->groupBox_Mesh->TabIndex = 20;
			this->groupBox_Mesh->TabStop = false;
			this->groupBox_Mesh->Text = L"Mesh";
			// 
			// trackBar_Alpha
			// 
			this->trackBar_Alpha->Location = System::Drawing::Point(5, 33);
			this->trackBar_Alpha->Maximum = 255;
			this->trackBar_Alpha->Minimum = 1;
			this->trackBar_Alpha->Name = L"trackBar_Alpha";
			this->trackBar_Alpha->Size = System::Drawing::Size(137, 45);
			this->trackBar_Alpha->TabIndex = 0;
			this->trackBar_Alpha->Value = 255;
			this->trackBar_Alpha->Scroll += gcnew System::EventHandler(this, &Form1::trackBar_Alpha_Scroll);
			// 
			// LBL_Alpha
			// 
			this->LBL_Alpha->AutoSize = true;
			this->LBL_Alpha->Location = System::Drawing::Point(4, 17);
			this->LBL_Alpha->Name = L"LBL_Alpha";
			this->LBL_Alpha->Size = System::Drawing::Size(34, 13);
			this->LBL_Alpha->TabIndex = 10;
			this->LBL_Alpha->Text = L"Alpha";
			// 
			// groupBox_Display_SDF
			// 
			this->groupBox_Display_SDF->Controls->Add(this->RB_Display_SDF_on_Vertices);
			this->groupBox_Display_SDF->Controls->Add(this->RB_Display_SDF_on_Faces);
			this->groupBox_Display_SDF->Location = System::Drawing::Point(8, 3);
			this->groupBox_Display_SDF->Name = L"groupBox_Display_SDF";
			this->groupBox_Display_SDF->Size = System::Drawing::Size(149, 46);
			this->groupBox_Display_SDF->TabIndex = 19;
			this->groupBox_Display_SDF->TabStop = false;
			this->groupBox_Display_SDF->Text = L"Display SDF";
			// 
			// RB_Display_SDF_on_Vertices
			// 
			this->RB_Display_SDF_on_Vertices->AutoSize = true;
			this->RB_Display_SDF_on_Vertices->Checked = true;
			this->RB_Display_SDF_on_Vertices->Location = System::Drawing::Point(68, 19);
			this->RB_Display_SDF_on_Vertices->Name = L"RB_Display_SDF_on_Vertices";
			this->RB_Display_SDF_on_Vertices->Size = System::Drawing::Size(63, 17);
			this->RB_Display_SDF_on_Vertices->TabIndex = 1;
			this->RB_Display_SDF_on_Vertices->TabStop = true;
			this->RB_Display_SDF_on_Vertices->Text = L"Vertices";
			this->RB_Display_SDF_on_Vertices->UseVisualStyleBackColor = true;
			this->RB_Display_SDF_on_Vertices->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Display_SDF_on_Vertices_CheckedChanged);
			// 
			// RB_Display_SDF_on_Faces
			// 
			this->RB_Display_SDF_on_Faces->AutoSize = true;
			this->RB_Display_SDF_on_Faces->Location = System::Drawing::Point(8, 19);
			this->RB_Display_SDF_on_Faces->Name = L"RB_Display_SDF_on_Faces";
			this->RB_Display_SDF_on_Faces->Size = System::Drawing::Size(54, 17);
			this->RB_Display_SDF_on_Faces->TabIndex = 0;
			this->RB_Display_SDF_on_Faces->Text = L"Faces";
			this->RB_Display_SDF_on_Faces->UseVisualStyleBackColor = true;
			this->RB_Display_SDF_on_Faces->CheckedChanged += gcnew System::EventHandler(this, &Form1::RB_Display_SDF_on_Faces_CheckedChanged);
			// 
			// groupBox_Status
			// 
			this->groupBox_Status->Controls->Add(this->TB_Status);
			this->groupBox_Status->Controls->Add(this->progressBar);
			this->groupBox_Status->Location = System::Drawing::Point(3, 455);
			this->groupBox_Status->Name = L"groupBox_Status";
			this->groupBox_Status->Size = System::Drawing::Size(168, 77);
			this->groupBox_Status->TabIndex = 11;
			this->groupBox_Status->TabStop = false;
			this->groupBox_Status->Text = L"Status";
			// 
			// TB_Status
			// 
			this->TB_Status->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->TB_Status->BackColor = System::Drawing::Color::Gainsboro;
			this->TB_Status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->TB_Status->Location = System::Drawing::Point(10, 19);
			this->TB_Status->Name = L"TB_Status";
			this->TB_Status->ReadOnly = true;
			this->TB_Status->Size = System::Drawing::Size(150, 20);
			this->TB_Status->TabIndex = 9;
			// 
			// progressBar
			// 
			this->progressBar->Location = System::Drawing::Point(10, 45);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(149, 23);
			this->progressBar->Step = 1;
			this->progressBar->TabIndex = 11;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 562);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Shape Diameter Function";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage_Info->ResumeLayout(false);
			this->tabPage_Info->PerformLayout();
			this->groupBox_Selected_Face->ResumeLayout(false);
			this->groupBox_Selected_Face->PerformLayout();
			this->groupBox_SDF->ResumeLayout(false);
			this->groupBox_SDF->PerformLayout();
			this->groupBox_Total->ResumeLayout(false);
			this->groupBox_Total->PerformLayout();
			this->tabPage_SDF->ResumeLayout(false);
			this->groupBox_GPU_Settings->ResumeLayout(false);
			this->groupBox_GPU_Settings->PerformLayout();
			this->groupBox_Params->ResumeLayout(false);
			this->groupBox_Params->PerformLayout();
			this->groupBox_Mode->ResumeLayout(false);
			this->groupBox_Mode->PerformLayout();
			this->tabPage_Visual->ResumeLayout(false);
			this->groupBox_Octree->ResumeLayout(false);
			this->groupBox_Octree->PerformLayout();
			this->groupBox_Normalization->ResumeLayout(false);
			this->groupBox_Normalization->PerformLayout();
			this->groupBox_Mesh->ResumeLayout(false);
			this->groupBox_Mesh->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar_Alpha))->EndInit();
			this->groupBox_Display_SDF->ResumeLayout(false);
			this->groupBox_Display_SDF->PerformLayout();
			this->groupBox_Status->ResumeLayout(false);
			this->groupBox_Status->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region Extra Functions

	private: System::Void SetOpenGLSize(int width, int height)
	{
		if(width<1) width = 1;
		if(height<1) height = 1;
		OpenGL->ResizeOpengl(width, height);
	}
	protected: virtual void Form1::WndProc(System::Windows::Forms::Message %m) override  
	{        
		if(m.Msg == WM_MOUSEWHEEL)  
		{
			OpenGL->WndProc(m);
		}
		System::Windows::Forms::Form::WndProc(m);  
	}

	private: void SetCheckedDrawMode(VISUAL_STATE d_mode)
	{
		TSMI_None->Checked = false;
		TSMI_Default->Checked = false;
		TSMI_Wireframe->Checked = false;
		TSMI_SDF_Hodnoty->Checked = false;
		TSMI_Picking_Hodnoty->Checked = false;
		switch(d_mode)
		{
			case VISUAL_NONE:		TSMI_None->Checked = true; break;
			case VISUAL_DEFAULT:	TSMI_Default->Checked = true; break;
			case VISUAL_WIREFRAME:	TSMI_Wireframe->Checked = true; break;
			case VISUAL_SDF:		TSMI_SDF_Hodnoty->Checked = true; break;
			case VISUAL_PICKING:	TSMI_Picking_Hodnoty->Checked = true; break;
			default: break;
		}
		Nastavenia->VISUAL_State = d_mode;
	}
	private: System::Void MyThreadProcedure()  
	{ 
		// Zavola vypocet SDF v zvlast Threade
		MController->ComputeSDF();
	}
#pragma endregion

#pragma region Special Functions
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		//timer
		OpenGL->Render();
		OpenGL->SwapOpenGLBuffers();

		int tmp = int(static_cast<void*>(MController->selected));
		this->TB_Face_ID->Text = "0 x" + tmp;
		if(MController->selected != NULL)
			this->TB_SDF_Value->Text = "" + MController->selected->quality->value;

		if(computing_SDF == true)
		{
			TB_Status->Text = "Computing SDF: " + Nastavenia->SDF_STATUS + "%";
			int val = (int)Nastavenia->SDF_STATUS - progressBar->Value;
			if(val > 0)
				progressBar->Increment(val);
			//progressBar->Step = val;
			//progressBar->PerformStep();
			//Nastavenia->SDF_STATUS += 1;
			if(Nastavenia->SDF_STATUS >= 100)
			{
				computing_SDF = false;
				TB_Status->Text = "SDF Computed";
				SetCheckedDrawMode(VISUAL_SDF);
				showToolStripMenuItem->Enabled = true;
				toolsToolStripMenuItem->Enabled = true;
				fileToolStripMenuItem->Enabled = true;

				this->TB_Min_SDF->Text = ""+Nastavenia->DEBUG_Min_SDF;
				this->TB_Max_SDF->Text = ""+Nastavenia->DEBUG_Max_SDF;
			}
		}
	}
	private: System::Void panel1_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		if(this->timer1->Enabled == false)
			return;
		Control^ control = dynamic_cast<Control^>(sender);
		SetOpenGLSize(control->Size.Width, control->Size.Height);
	}
#pragma endregion

#pragma region SDF Parameters

	private: System::Void RB_SDF_Mode_CPU_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_SDF_Mode_CPU->Checked == true)
		{
			Nastavenia->SDF_Mode = SDF_CPU;
			RB_SDF_Mode_GPU->Checked = false;
			RB_SDF_Mode_Debug_GPU->Checked = false;
		}
	}
	private: System::Void RB_SDF_Mode_GPU_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_SDF_Mode_GPU->Checked == true)
		{
			Nastavenia->SDF_Mode = SDF_GPU;
			RB_SDF_Mode_CPU->Checked = false;
			RB_SDF_Mode_Debug_GPU->Checked = false;
		}
	}
	private: System::Void RB_SDF_Mode_Debug_GPU_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_SDF_Mode_Debug_GPU->Checked == true)
		{
			Nastavenia->SDF_Mode = SDF_GPU_DEBUG;
			RB_SDF_Mode_CPU->Checked = false;
			RB_SDF_Mode_GPU->Checked = false;
		}
	}
	private: System::Void TB_Rays_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Int32 value = 0;
		if(System::Int32::TryParse(TB_Rays->Text, value))
		{
			if(value < 1)
				value = 1;
			if(value > 1024)
				value = 1024;
			Nastavenia->SDF_Rays = (unsigned int)value;
		}
		else
		{
			Nastavenia->SDF_Rays = 30;
		}
	}
	private: System::Void TB_Cone_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Double value = 0;
		if(System::Double::TryParse(TB_Cone->Text, value))
		{
			if(value < 2.0)
				value = 2.0;
			if(value > 360.0)
				value = 360.0;
			Nastavenia->SDF_Cone = (float)value;
		}
		else
		{
			Nastavenia->SDF_Cone = 120.0f;
		}
	}
	private: System::Void TB_Radius_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Int32 value = 0;
		if(System::Int32::TryParse(TB_Radius->Text, value))
		{
			if(value < 0)
				value = 0;
			if(value > 20)
				value = 20;
			Nastavenia->SDF_Smoothing_Radius = (unsigned int)value;
		}
		else
		{
			Nastavenia->SDF_Smoothing_Radius = 2;
		}
	}
	private: System::Void CHB_Reverted_Rays_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->SDF_Revert_Rays = CHB_Reverted_Rays->Checked;
	}
	private: System::Void CHB_Uniform_Distribution_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->SDF_Distribution = CHB_Uniform_Distribution->Checked;
	}
	private: System::Void TB_Work_Groups_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Double value = 0;
		if(System::Double::TryParse(TB_Work_Groups->Text, value))
		{
			if(value < 0.1)
				value = 0.1;
			if(value > 10.0)
				value = 10.0;
			Nastavenia->GPU_Work_Groups = (float)value;
		}
		else
		{
			Nastavenia->GPU_Work_Groups = 2.0f;
		}
	}
	private: System::Void TB_Work_Items_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Double value = 0;
		if(System::Double::TryParse(TB_Work_Items->Text, value))
		{
			if(value < 0.01)
				value = 0.01;
			if(value > 1.0)
				value = 1.0;
			Nastavenia->GPU_Work_Items= (float)value;
		}
		else
		{
			Nastavenia->GPU_Work_Items = 0.25f;
		}
	}
#pragma endregion

#pragma region Display Parameters
	private: System::Void RB_Display_SDF_on_Faces_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Display_SDF_on_Faces->Checked == true)
		{
			Nastavenia->VISUAL_Smoothed = false;
			RB_Display_SDF_on_Vertices->Checked = false;
		}
	}
	private: System::Void RB_Display_SDF_on_Vertices_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Display_SDF_on_Vertices->Checked == true)
		{
			Nastavenia->VISUAL_Smoothed = true;
			RB_Display_SDF_on_Faces->Checked = false;
		}
	}
	private: System::Void RB_Norm1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Norm1->Checked == true)
		{
			Nastavenia->VISUAL_SDF_Type = VISUAL_NORMALIZED_0_1;
			RB_Norm2->Checked = false;
			RB_Norm3->Checked = false;
			RB_Norm4->Checked = false;
		}
	}
	private: System::Void RB_Norm2_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Norm2->Checked == true)
		{
			Nastavenia->VISUAL_SDF_Type = VISUAL_NORMALIZED_MIN_1;
			RB_Norm1->Checked = false;
			RB_Norm3->Checked = false;
			RB_Norm4->Checked = false;
		}
	}
	private: System::Void RB_Norm3_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Norm3->Checked == true)
		{
			Nastavenia->VISUAL_SDF_Type = VISUAL_NORMALIZED_0_MAX;
			RB_Norm1->Checked = false;
			RB_Norm2->Checked = false;
			RB_Norm4->Checked = false;
		}
	}
	private: System::Void RB_Norm4_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(RB_Norm4->Checked == true)
		{
			Nastavenia->VISUAL_SDF_Type = VISUAL_NORMALIZED_MIN_MAX;
			RB_Norm1->Checked = false;
			RB_Norm2->Checked = false;
			RB_Norm3->Checked = false;
		}
	}
	private: System::Void trackBar_Alpha_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->VISUAL_Alpha = (unsigned int)trackBar_Alpha->Value;
	}
	private: System::Void TB_Depth_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Int32 value = 0;
		if(System::Int32::TryParse(TB_Depth->Text, value))
		{
			if(value < 1)
				value = 1;
			if(value > 30)
				value = 30;
			Nastavenia->OCTREE_Depth = (unsigned int)value;
		}
		else
		{
			Nastavenia->OCTREE_Depth = 8;
		}
	}
	private: System::Void TB_Threshold_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		Int32 value = 0;
		if(System::Int32::TryParse(TB_Threshold->Text, value))
		{
			if(value < 1)
				value = 1;
			if(value > 1000)
				value = 1000;
			Nastavenia->OCTREE_Threshold = (unsigned int)value;
		}
		else
		{
			Nastavenia->OCTREE_Threshold = 4;
		}
	}
#pragma endregion

#pragma region File
	private: System::Void TSMI_Open_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Enabled = false;
		TB_Status->Text = "Opening File";

		// Displays an OpenFileDialog so the user can select a model.
		OpenFileDialog ^openFileDialog1 = gcnew OpenFileDialog();
		openFileDialog1->Filter =	"Common 3D Model Formats|*.obj;*.3ds;*.dae;*.blend;*.ase;*.ifc;*.xgl;*.zgl;*.ply;*.dxf;*.lwo;*.lws;*.lxo;*.stl;*.x;*.ac;*.ms3d;*.cob;*.scn"+
									"|VCG Format|*.ply;"+
									"|Motion Capture Formats|*.bvh;*.csm"+
									"|Graphics Engine Formats|*.xml;*.irrmesh;*.irr"+
									"|Game File Formats|*.mdl;*.md2;*.md3;*.pk3;*.mdc;*.md5;*.smd;*.vta;*.m3;*.3d"+
									"|Other File Formats|*.b3d;*.q3d;*.q3s;*.nff;*.off;*.raw;*.ter;*.mdl;*.hmp;*.ndo"+
									"|All files (*.*)|*.*";
		openFileDialog1->Title = "Select Model";

		// Show the Dialog.
		// If the user clicked OK in the dialog and
		// a .xml file was selected, open it.
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			Nastavenia->DEBUG_Min_SDF = 0;
			Nastavenia->DEBUG_Max_SDF = 0;
			TB_Status->Text = "Loading File";
			String^ extension = System::IO::Path::GetExtension(openFileDialog1->FileName);
			//if((extension == ".ply") ||(extension == ".stl") ||(extension == ".obj") ||(extension == ".off") ||(extension == ".vmi"))
			if(extension == ".ply")
				MController->LoadFileVCG(MarshalString(openFileDialog1->FileName));
			else
				MController->LoadFile(MarshalString(openFileDialog1->FileName));
			OpenGL->ReloadBoundary();
			this->TB_Filename->Text = System::IO::Path::GetFileName(openFileDialog1->FileName);
			this->TB_Total_Triangles->Text = ""+Nastavenia->INFO_Total_Triangles;
			this->TB_Total_Vertices->Text = ""+Nastavenia->INFO_Total_Vertices;
			this->TB_Min_SDF->Text = "0";
			this->TB_Max_SDF->Text = "0";
			this->showToolStripMenuItem->Enabled = true;
			this->toolsToolStripMenuItem->Enabled = true;

			this->TB_Min_SDF->Text = ""+Nastavenia->DEBUG_Min_SDF;
			this->TB_Max_SDF->Text = ""+Nastavenia->DEBUG_Max_SDF;

			MController->logInfo("File: "+ MarshalString(openFileDialog1->FileName) + " Loaded");
			TB_Status->Text = "File Loaded";
			SetCheckedDrawMode(VISUAL_DEFAULT);
		}
		timer1->Enabled = true;
	}
	private: System::Void TSMI_Save_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//TODO cez tu druhu libku
		//PLY, STL, OFF, OBJ, 3DS, COLLADA, VRML, DXF, GTS, U3D, IDTF, X3D
	}
	private: System::Void TSMI_Save_As_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//TODO cez tu druhu libku
	}
	private: System::Void TSMI_Export_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Enabled = false;
		float* values1 = NULL;
		float* values2 = NULL;
		int size = 0;

		MController->logInfo("Exporting SDF values");
		values1 = MController->GetSDF(size, false);
		values2 = MController->GetSDF(size, true);
		std::ofstream vypis;
		vypis.open ("Export.txt");
		for(int i = 0; i < size; i++)
		{
			vypis << MarshalString(values1[i] + " " + values2[i] + "\n");
		}
		vypis.close();

		MController->logInfo("SDF values Exported");
		TB_Status->Text = "SDF values Exported";
		delete [] values1;
		delete [] values2;
		timer1->Enabled = true;
	}
	private: System::Void TSMI_Exit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}
#pragma endregion

#pragma region Show
	private: System::Void TSMI_None_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(VISUAL_NONE);
		MController->logInfo("Skry Mesh");
	}
	private: System::Void TSMI_Default_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(VISUAL_DEFAULT);
		MController->logInfo("Zobrazi Default");
	}
	private: System::Void TSMI_Wireframe_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(VISUAL_WIREFRAME);
		MController->logInfo("Zobrazi Wireframe");
	}
	private: System::Void TSMI_SDF_Hodnoty_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(VISUAL_SDF);
		MController->logInfo("Zobrazi SDF hodnoty");
	}
	private: System::Void TSMI_Picking_Hodnoty_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(VISUAL_PICKING);
		MController->logInfo("Zobrazi Picking hodnoty");
	}
	private: System::Void TSMI_Octree_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->VISUAL_Octree = (Nastavenia->VISUAL_Octree == true ? false : true);
		TSMI_Octree->Checked = Nastavenia->VISUAL_Octree;
		MController->logInfo("Zobrazi Octree");
	}
	private: System::Void TSMI_Normals_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->VISUAL_Normals = (Nastavenia->VISUAL_Normals == true ? false : true);
		TSMI_Normals->Checked = Nastavenia->VISUAL_Normals;
		MController->logInfo("Zobrazi Normaly");
	}
	private: System::Void TSMI_Axes_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Nastavenia->VISUAL_Axes = (Nastavenia->VISUAL_Axes == true ? false : true);
		TSMI_Axes->Checked = Nastavenia->VISUAL_Axes;
		MController->logInfo("Zobrazi Osy");
	}
#pragma endregion

#pragma region Tools
	private: System::Void TSMI_Compute_Octree_Click(System::Object^  sender, System::EventArgs^  e)
	{
		MController->ReloadOctreeData();
	}
	private: System::Void TSMI_Compute_SDF_Click(System::Object^  sender, System::EventArgs^  e)
	{
		progressBar->Value = 0;
		TB_Min_SDF->Text = "0";
		TB_Max_SDF->Text = "0";
		// bezi v threade aby som mohol updatovat popis
		MController->logInfo("Compute SDF");
		myThread = gcnew Thread(gcnew ThreadStart(this,&Form1::MyThreadProcedure));  
		myThread->Start();

		computing_SDF = true;
		showToolStripMenuItem->Enabled = false;
		toolsToolStripMenuItem->Enabled = false;
		fileToolStripMenuItem->Enabled = false;
	}
	private: System::Void TSMI_Compute_Triangulation_Click(System::Object^  sender, System::EventArgs^  e)
	{
		MController->TriangulatePoints();
		this->TB_Total_Triangles->Text = ""+Nastavenia->INFO_Total_Triangles;
	}
#pragma endregion

#pragma region About
	private: System::Void TSMI_about_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SettingsForm^ form2=gcnew SettingsForm();
		form2->ShowDialog();
	}
#pragma endregion
};
}

