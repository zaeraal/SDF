#pragma once

namespace SDF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SettingsForm
	/// </summary>
	public ref class SettingsForm : public System::Windows::Forms::Form
	{
	public:
		SettingsForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettingsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  BTN_Done;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->BTN_Done = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// BTN_Done
			// 
			this->BTN_Done->Location = System::Drawing::Point(130, 162);
			this->BTN_Done->Name = L"BTN_Done";
			this->BTN_Done->Size = System::Drawing::Size(75, 23);
			this->BTN_Done->TabIndex = 0;
			this->BTN_Done->Text = L"Done";
			this->BTN_Done->UseVisualStyleBackColor = true;
			this->BTN_Done->Click += gcnew System::EventHandler(this, &SettingsForm::BTN_Done_Click);
			// 
			// SettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Controls->Add(this->BTN_Done);
			this->Name = L"SettingsForm";
			this->Text = L"SettingsForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void BTN_Done_Click(System::Object^  sender, System::EventArgs^  e) {
				 Close();
			 }
	};
}
