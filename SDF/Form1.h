#pragma once

#include "StringHelper.h"
#include "Octree.h"
#include "Assimp.h"
#include "OpenGL.h"

namespace SDF {

	using namespace System;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace OpenGLForm;
	using namespace AssimpFileHandler;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			Assimp = new CAssimp();
			InitializeComponent();
			OpenGL = gcnew COpenGL(this->panel1, Assimp);

			this->timer1->Enabled = true;
			Assimp->logInfo("Form Fully Loaded");
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
				delete Assimp;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		CAssimp* Assimp;
		COpenGL^ OpenGL;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  souborToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  novýToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  otevøítToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator;
	private: System::Windows::Forms::ToolStripMenuItem^  uložitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  uložitjakoToolStripMenuItem;



	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  ukonèitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  upravitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  zpìtToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  znovuToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  vyjmoutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  kopírovatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  vložitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripMenuItem^  vybratvšeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  nástrojeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pøizpùsobitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  možnostiToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  nápovìdaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  obsahToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  indexToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  hledatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
	private: System::Windows::Forms::ToolStripMenuItem^  oproduktuToolStripMenuItem;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Panel^  panel1;
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
			this->souborToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->novýToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->otevøítToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->uložitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->uložitjakoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->ukonèitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->upravitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->zpìtToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->znovuToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->vyjmoutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->kopírovatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->vložitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->vybratvšeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nástrojeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pøizpùsobitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->možnostiToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nápovìdaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->obsahToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->indexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hledatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->oproduktuToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->souborToolStripMenuItem, 
				this->upravitToolStripMenuItem, this->nástrojeToolStripMenuItem, this->nápovìdaToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(784, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// souborToolStripMenuItem
			// 
			this->souborToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->novýToolStripMenuItem, 
				this->otevøítToolStripMenuItem, this->toolStripSeparator, this->uložitToolStripMenuItem, this->uložitjakoToolStripMenuItem, this->toolStripSeparator2, 
				this->ukonèitToolStripMenuItem});
			this->souborToolStripMenuItem->Name = L"souborToolStripMenuItem";
			this->souborToolStripMenuItem->Size = System::Drawing::Size(57, 20);
			this->souborToolStripMenuItem->Text = L"&Soubor";
			// 
			// novýToolStripMenuItem
			// 
			this->novýToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"novýToolStripMenuItem.Image")));
			this->novýToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->novýToolStripMenuItem->Name = L"novýToolStripMenuItem";
			this->novýToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->novýToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->novýToolStripMenuItem->Text = L"&Nový";
			// 
			// otevøítToolStripMenuItem
			// 
			this->otevøítToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"otevøítToolStripMenuItem.Image")));
			this->otevøítToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->otevøítToolStripMenuItem->Name = L"otevøítToolStripMenuItem";
			this->otevøítToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->otevøítToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->otevøítToolStripMenuItem->Text = L"&Otevøít";
			this->otevøítToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::otevøítToolStripMenuItem_Click);
			// 
			// toolStripSeparator
			// 
			this->toolStripSeparator->Name = L"toolStripSeparator";
			this->toolStripSeparator->Size = System::Drawing::Size(150, 6);
			// 
			// uložitToolStripMenuItem
			// 
			this->uložitToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"uložitToolStripMenuItem.Image")));
			this->uložitToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->uložitToolStripMenuItem->Name = L"uložitToolStripMenuItem";
			this->uložitToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->uložitToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->uložitToolStripMenuItem->Text = L"&Uložit";
			// 
			// uložitjakoToolStripMenuItem
			// 
			this->uložitjakoToolStripMenuItem->Name = L"uložitjakoToolStripMenuItem";
			this->uložitjakoToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->uložitjakoToolStripMenuItem->Text = L"Uložit j&ako";
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(150, 6);
			// 
			// ukonèitToolStripMenuItem
			// 
			this->ukonèitToolStripMenuItem->Name = L"ukonèitToolStripMenuItem";
			this->ukonèitToolStripMenuItem->Size = System::Drawing::Size(153, 22);
			this->ukonèitToolStripMenuItem->Text = L"U&konèit";
			this->ukonèitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ukonèitToolStripMenuItem_Click);
			// 
			// upravitToolStripMenuItem
			// 
			this->upravitToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->zpìtToolStripMenuItem, 
				this->znovuToolStripMenuItem, this->toolStripSeparator3, this->vyjmoutToolStripMenuItem, this->kopírovatToolStripMenuItem, this->vložitToolStripMenuItem, 
				this->toolStripSeparator4, this->vybratvšeToolStripMenuItem});
			this->upravitToolStripMenuItem->Name = L"upravitToolStripMenuItem";
			this->upravitToolStripMenuItem->Size = System::Drawing::Size(57, 20);
			this->upravitToolStripMenuItem->Text = L"&Upravit";
			// 
			// zpìtToolStripMenuItem
			// 
			this->zpìtToolStripMenuItem->Name = L"zpìtToolStripMenuItem";
			this->zpìtToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z));
			this->zpìtToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->zpìtToolStripMenuItem->Text = L"&Zpìt";
			// 
			// znovuToolStripMenuItem
			// 
			this->znovuToolStripMenuItem->Name = L"znovuToolStripMenuItem";
			this->znovuToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y));
			this->znovuToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->znovuToolStripMenuItem->Text = L"&Znovu";
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(164, 6);
			// 
			// vyjmoutToolStripMenuItem
			// 
			this->vyjmoutToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"vyjmoutToolStripMenuItem.Image")));
			this->vyjmoutToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->vyjmoutToolStripMenuItem->Name = L"vyjmoutToolStripMenuItem";
			this->vyjmoutToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X));
			this->vyjmoutToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->vyjmoutToolStripMenuItem->Text = L"&Vyjmout";
			// 
			// kopírovatToolStripMenuItem
			// 
			this->kopírovatToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"kopírovatToolStripMenuItem.Image")));
			this->kopírovatToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->kopírovatToolStripMenuItem->Name = L"kopírovatToolStripMenuItem";
			this->kopírovatToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->kopírovatToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->kopírovatToolStripMenuItem->Text = L"&Kopírovat";
			// 
			// vložitToolStripMenuItem
			// 
			this->vložitToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"vložitToolStripMenuItem.Image")));
			this->vložitToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->vložitToolStripMenuItem->Name = L"vložitToolStripMenuItem";
			this->vložitToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::V));
			this->vložitToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->vložitToolStripMenuItem->Text = L"&Vložit";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(164, 6);
			// 
			// vybratvšeToolStripMenuItem
			// 
			this->vybratvšeToolStripMenuItem->Name = L"vybratvšeToolStripMenuItem";
			this->vybratvšeToolStripMenuItem->Size = System::Drawing::Size(167, 22);
			this->vybratvšeToolStripMenuItem->Text = L"&Vybrat vše";
			// 
			// nástrojeToolStripMenuItem
			// 
			this->nástrojeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->pøizpùsobitToolStripMenuItem, 
				this->možnostiToolStripMenuItem});
			this->nástrojeToolStripMenuItem->Name = L"nástrojeToolStripMenuItem";
			this->nástrojeToolStripMenuItem->Size = System::Drawing::Size(63, 20);
			this->nástrojeToolStripMenuItem->Text = L"&Nástroje";
			// 
			// pøizpùsobitToolStripMenuItem
			// 
			this->pøizpùsobitToolStripMenuItem->Name = L"pøizpùsobitToolStripMenuItem";
			this->pøizpùsobitToolStripMenuItem->Size = System::Drawing::Size(133, 22);
			this->pøizpùsobitToolStripMenuItem->Text = L"&Pøizpùsobit";
			// 
			// možnostiToolStripMenuItem
			// 
			this->možnostiToolStripMenuItem->Name = L"možnostiToolStripMenuItem";
			this->možnostiToolStripMenuItem->Size = System::Drawing::Size(133, 22);
			this->možnostiToolStripMenuItem->Text = L"&Možnosti";
			// 
			// nápovìdaToolStripMenuItem
			// 
			this->nápovìdaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->obsahToolStripMenuItem, 
				this->indexToolStripMenuItem, this->hledatToolStripMenuItem, this->toolStripSeparator5, this->oproduktuToolStripMenuItem});
			this->nápovìdaToolStripMenuItem->Name = L"nápovìdaToolStripMenuItem";
			this->nápovìdaToolStripMenuItem->Size = System::Drawing::Size(73, 20);
			this->nápovìdaToolStripMenuItem->Text = L"&Nápovìda";
			// 
			// obsahToolStripMenuItem
			// 
			this->obsahToolStripMenuItem->Name = L"obsahToolStripMenuItem";
			this->obsahToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->obsahToolStripMenuItem->Text = L"&Obsah";
			// 
			// indexToolStripMenuItem
			// 
			this->indexToolStripMenuItem->Name = L"indexToolStripMenuItem";
			this->indexToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->indexToolStripMenuItem->Text = L"&Index";
			// 
			// hledatToolStripMenuItem
			// 
			this->hledatToolStripMenuItem->Name = L"hledatToolStripMenuItem";
			this->hledatToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->hledatToolStripMenuItem->Text = L"&Hledat";
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(141, 6);
			// 
			// oproduktuToolStripMenuItem
			// 
			this->oproduktuToolStripMenuItem->Name = L"oproduktuToolStripMenuItem";
			this->oproduktuToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->oproduktuToolStripMenuItem->Text = L"&O produktu...";
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
			this->panel1->Location = System::Drawing::Point(0, 27);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(784, 535);
			this->panel1->TabIndex = 1;
			this->panel1->Resize += gcnew System::EventHandler(this, &Form1::panel1_Resize);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 562);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Shape Diameter Function";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
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

#pragma endregion

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		//timer
		OpenGL->Render();
		OpenGL->SwapOpenGLBuffers();
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
		// Displays an OpenFileDialog so the user can select a model.
		OpenFileDialog ^openFileDialog1 = gcnew OpenFileDialog();
		openFileDialog1->Filter = "3D Model Files(*.OBJ)|*.OBJ|All files (*.*)|*.*";
		openFileDialog1->Title = "Select Model";

		// Show the Dialog.
		// If the user clicked OK in the dialog and
		// a .xml file was selected, open it.
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			//OpenGL->SetFileName(openFileDialog1->FileName);
			//OpenGL->LoadFromFile();
			//this->Text = gcnew System::String(OpenGL->GetPanoName().c_str());
			Assimp->logInfo("File: "+ MarshalString(openFileDialog1->FileName) + " Loaded");
		}
	}
	private: System::Void ukonèitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}
};
}

