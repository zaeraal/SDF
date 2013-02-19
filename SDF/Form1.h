#pragma once

#include "Model.h"
#include "OpenGL.h"
#include "SettingsForm.h"

namespace SDF {

	using namespace System;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
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

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;



	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveasToolStripMenuItem;





	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  showToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  optionsToolStripMenuItem;












	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;






	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::TextBox^  TB_Filename;
	private: System::Windows::Forms::Label^  LBL_Filename;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  LBL_SDF;
	private: System::Windows::Forms::TextBox^  TB_Triangle;
	private: System::Windows::Forms::Label^  LBL_Triangle;
	private: System::Windows::Forms::TextBox^  TB_Total;
	private: System::Windows::Forms::Label^  LBL_Total;
	private: System::Windows::Forms::ToolStripMenuItem^  computeSDFToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^  octreeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sDFHodnotyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  wireframeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  defaultToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  pickingHodnotyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::TextBox^  TB_Status;
	private: System::Windows::Forms::Label^  LBL_Status;
	private: System::Windows::Forms::ToolStripMenuItem^  normalsToolStripMenuItem;


	private: System::ComponentModel::IContainer^  components;


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
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveasToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->defaultToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wireframeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sDFHodnotyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pickingHodnotyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->octreeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normalsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->computeSDFToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->TB_Status = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Status = (gcnew System::Windows::Forms::Label());
			this->TB_Total = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Total = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->LBL_SDF = (gcnew System::Windows::Forms::Label());
			this->TB_Triangle = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Triangle = (gcnew System::Windows::Forms::Label());
			this->TB_Filename = (gcnew System::Windows::Forms::TextBox());
			this->LBL_Filename = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->panel2->SuspendLayout();
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
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->openToolStripMenuItem, 
				this->toolStripSeparator, this->saveToolStripMenuItem, this->saveasToolStripMenuItem, this->toolStripSeparator2, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"openToolStripMenuItem.Image")));
			this->openToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->openToolStripMenuItem->Text = L"&Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::otevøítToolStripMenuItem_Click);
			// 
			// toolStripSeparator
			// 
			this->toolStripSeparator->Name = L"toolStripSeparator";
			this->toolStripSeparator->Size = System::Drawing::Size(143, 6);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"saveToolStripMenuItem.Image")));
			this->saveToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->saveToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->saveToolStripMenuItem->Text = L"&Save";
			// 
			// saveasToolStripMenuItem
			// 
			this->saveasToolStripMenuItem->Name = L"saveasToolStripMenuItem";
			this->saveasToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->saveasToolStripMenuItem->Text = L"Save &as";
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(143, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->exitToolStripMenuItem->Text = L"E&xit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ukonèitToolStripMenuItem_Click);
			// 
			// showToolStripMenuItem
			// 
			this->showToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->defaultToolStripMenuItem, 
				this->wireframeToolStripMenuItem, this->sDFHodnotyToolStripMenuItem, this->pickingHodnotyToolStripMenuItem, this->toolStripSeparator1, 
				this->octreeToolStripMenuItem, this->normalsToolStripMenuItem});
			this->showToolStripMenuItem->Enabled = false;
			this->showToolStripMenuItem->Name = L"showToolStripMenuItem";
			this->showToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->showToolStripMenuItem->Text = L"Show";
			// 
			// defaultToolStripMenuItem
			// 
			this->defaultToolStripMenuItem->Name = L"defaultToolStripMenuItem";
			this->defaultToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->defaultToolStripMenuItem->Text = L"&Default";
			this->defaultToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::normalToolStripMenuItem_Click);
			// 
			// wireframeToolStripMenuItem
			// 
			this->wireframeToolStripMenuItem->Name = L"wireframeToolStripMenuItem";
			this->wireframeToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->wireframeToolStripMenuItem->Text = L"&Wireframe";
			this->wireframeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::wireframeToolStripMenuItem_Click);
			// 
			// sDFHodnotyToolStripMenuItem
			// 
			this->sDFHodnotyToolStripMenuItem->Name = L"sDFHodnotyToolStripMenuItem";
			this->sDFHodnotyToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->sDFHodnotyToolStripMenuItem->Text = L"&SDF values";
			this->sDFHodnotyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sDFHodnotyToolStripMenuItem_Click);
			// 
			// pickingHodnotyToolStripMenuItem
			// 
			this->pickingHodnotyToolStripMenuItem->Name = L"pickingHodnotyToolStripMenuItem";
			this->pickingHodnotyToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->pickingHodnotyToolStripMenuItem->Text = L"&Picking values";
			this->pickingHodnotyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pickingHodnotyToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// octreeToolStripMenuItem
			// 
			this->octreeToolStripMenuItem->Name = L"octreeToolStripMenuItem";
			this->octreeToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->octreeToolStripMenuItem->Text = L"&Octree";
			this->octreeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::octreeToolStripMenuItem_Click);
			// 
			// normalsToolStripMenuItem
			// 
			this->normalsToolStripMenuItem->Name = L"normalsToolStripMenuItem";
			this->normalsToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->normalsToolStripMenuItem->Text = L"&Normals";
			this->normalsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::normalToolStripMenuItem1_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->computeSDFToolStripMenuItem, 
				this->optionsToolStripMenuItem});
			this->toolsToolStripMenuItem->Enabled = false;
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"&Tools";
			// 
			// computeSDFToolStripMenuItem
			// 
			this->computeSDFToolStripMenuItem->Name = L"computeSDFToolStripMenuItem";
			this->computeSDFToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->computeSDFToolStripMenuItem->Text = L"&Compute SDF";
			// 
			// optionsToolStripMenuItem
			// 
			this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
			this->optionsToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->optionsToolStripMenuItem->Text = L"&Options";
			this->optionsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::optionsToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->aboutToolStripMenuItem->Text = L"&About...";
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
			this->panel2->Controls->Add(this->TB_Status);
			this->panel2->Controls->Add(this->LBL_Status);
			this->panel2->Controls->Add(this->TB_Total);
			this->panel2->Controls->Add(this->LBL_Total);
			this->panel2->Controls->Add(this->textBox1);
			this->panel2->Controls->Add(this->LBL_SDF);
			this->panel2->Controls->Add(this->TB_Triangle);
			this->panel2->Controls->Add(this->LBL_Triangle);
			this->panel2->Controls->Add(this->TB_Filename);
			this->panel2->Controls->Add(this->LBL_Filename);
			this->panel2->Location = System::Drawing::Point(0, 27);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(174, 535);
			this->panel2->TabIndex = 2;
			// 
			// TB_Status
			// 
			this->TB_Status->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->TB_Status->Location = System::Drawing::Point(21, 509);
			this->TB_Status->Name = L"TB_Status";
			this->TB_Status->ReadOnly = true;
			this->TB_Status->Size = System::Drawing::Size(150, 20);
			this->TB_Status->TabIndex = 9;
			// 
			// LBL_Status
			// 
			this->LBL_Status->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->LBL_Status->AutoSize = true;
			this->LBL_Status->Location = System::Drawing::Point(18, 492);
			this->LBL_Status->Name = L"LBL_Status";
			this->LBL_Status->Size = System::Drawing::Size(37, 13);
			this->LBL_Status->TabIndex = 8;
			this->LBL_Status->Text = L"Status";
			// 
			// TB_Total
			// 
			this->TB_Total->Location = System::Drawing::Point(21, 75);
			this->TB_Total->Name = L"TB_Total";
			this->TB_Total->ReadOnly = true;
			this->TB_Total->Size = System::Drawing::Size(150, 20);
			this->TB_Total->TabIndex = 7;
			this->TB_Total->Text = L"0";
			// 
			// LBL_Total
			// 
			this->LBL_Total->AutoSize = true;
			this->LBL_Total->Location = System::Drawing::Point(18, 58);
			this->LBL_Total->Name = L"LBL_Total";
			this->LBL_Total->Size = System::Drawing::Size(77, 13);
			this->LBL_Total->TabIndex = 6;
			this->LBL_Total->Text = L"Total Triangles";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(21, 155);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(150, 20);
			this->textBox1->TabIndex = 5;
			this->textBox1->Text = L"0";
			// 
			// LBL_SDF
			// 
			this->LBL_SDF->AutoSize = true;
			this->LBL_SDF->Location = System::Drawing::Point(18, 138);
			this->LBL_SDF->Name = L"LBL_SDF";
			this->LBL_SDF->Size = System::Drawing::Size(58, 13);
			this->LBL_SDF->TabIndex = 4;
			this->LBL_SDF->Text = L"SDF Value";
			// 
			// TB_Triangle
			// 
			this->TB_Triangle->Location = System::Drawing::Point(21, 115);
			this->TB_Triangle->Name = L"TB_Triangle";
			this->TB_Triangle->ReadOnly = true;
			this->TB_Triangle->Size = System::Drawing::Size(150, 20);
			this->TB_Triangle->TabIndex = 3;
			this->TB_Triangle->Text = L"-1";
			// 
			// LBL_Triangle
			// 
			this->LBL_Triangle->AutoSize = true;
			this->LBL_Triangle->Location = System::Drawing::Point(18, 98);
			this->LBL_Triangle->Name = L"LBL_Triangle";
			this->LBL_Triangle->Size = System::Drawing::Size(104, 13);
			this->LBL_Triangle->TabIndex = 2;
			this->LBL_Triangle->Text = L"Selected Triangle ID";
			// 
			// TB_Filename
			// 
			this->TB_Filename->Location = System::Drawing::Point(21, 35);
			this->TB_Filename->Name = L"TB_Filename";
			this->TB_Filename->ReadOnly = true;
			this->TB_Filename->Size = System::Drawing::Size(150, 20);
			this->TB_Filename->TabIndex = 1;
			// 
			// LBL_Filename
			// 
			this->LBL_Filename->AutoSize = true;
			this->LBL_Filename->Location = System::Drawing::Point(18, 19);
			this->LBL_Filename->Name = L"LBL_Filename";
			this->LBL_Filename->Size = System::Drawing::Size(49, 13);
			this->LBL_Filename->TabIndex = 0;
			this->LBL_Filename->Text = L"Filename";
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
			this->panel2->PerformLayout();
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

	private: void SetCheckedDrawMode(int d_mode)
	{
		defaultToolStripMenuItem->Checked = false;
		wireframeToolStripMenuItem->Checked = false;
		sDFHodnotyToolStripMenuItem->Checked = false;
		pickingHodnotyToolStripMenuItem->Checked = false;
		switch(d_mode)
		{
			case 0: pickingHodnotyToolStripMenuItem->Checked = true; break;
			case 1: defaultToolStripMenuItem->Checked = true; break;
			case 2: sDFHodnotyToolStripMenuItem->Checked = true; break;
			case 3: wireframeToolStripMenuItem->Checked = true; break;
			default: break;
		}
	}
#pragma endregion

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		//timer
		OpenGL->Render();
		OpenGL->SwapOpenGLBuffers();

		int tmp = int(static_cast<void*>(MController->selected));
		this->TB_Triangle->Text = "0 x" + tmp;
	}
	private: System::Void panel1_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		if(this->timer1->Enabled == false)
			return;
		Control^ control = dynamic_cast<Control^>(sender);
		SetOpenGLSize(control->Size.Width, control->Size.Height);
	}
	private: System::Void otevøítToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Enabled = false;
		TB_Status->Text = "Opening File";

		// Displays an OpenFileDialog so the user can select a model.
		OpenFileDialog ^openFileDialog1 = gcnew OpenFileDialog();
		openFileDialog1->Filter = "3D Model Files(*.OBJ)|*.OBJ|All files (*.*)|*.*";
		openFileDialog1->Title = "Select Model";

		// Show the Dialog.
		// If the user clicked OK in the dialog and
		// a .xml file was selected, open it.
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			TB_Status->Text = "Loading File";
			MController->LoadFile(MarshalString(openFileDialog1->FileName));
			OpenGL->ReloadBoundary();
			this->TB_Filename->Text = System::IO::Path::GetFileName(openFileDialog1->FileName);
			this->TB_Total->Text = ""+MController->GetTriangleCount();

			MController->logInfo("File: "+ MarshalString(openFileDialog1->FileName) + " Loaded");
			TB_Status->Text = "File Loaded";
		}

		showToolStripMenuItem->Enabled = true;
		toolsToolStripMenuItem->Enabled = true;

		octreeToolStripMenuItem->Checked = false;
		normalsToolStripMenuItem->Checked = false;

		SetCheckedDrawMode(1);
		timer1->Enabled = true;
	}
	private: System::Void ukonèitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}
	private: System::Void octreeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		MController->show_octree = (MController->show_octree == true ? false : true);
		octreeToolStripMenuItem->Checked = MController->show_octree;
		MController->logInfo("Zobrazi Octree");
	}
	private: System::Void normalToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		MController->show_normals = (MController->show_normals == true ? false : true);
		normalsToolStripMenuItem->Checked = MController->show_normals;
		MController->logInfo("Zobrazi Normaly");
	}
	private: System::Void wireframeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(3);
		MController->setDrawMode(3);
		MController->logInfo("Zobrazi Wireframe");
	}
	private: System::Void normalToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(1);
		MController->setDrawMode(1);
		MController->logInfo("Zobrazi Default");
	}
	private: System::Void sDFHodnotyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(2);
		MController->setDrawMode(2);
		MController->logInfo("Zobrazi SDF hodnoty");
	}
	private: System::Void pickingHodnotyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCheckedDrawMode(0);
		MController->setDrawMode(0);
		MController->logInfo("Zobrazi Picking hodnoty");
	}
	private: System::Void optionsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SettingsForm^ form2=gcnew SettingsForm();
		form2->ShowDialog();
	}

};
}

