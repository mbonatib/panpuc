<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="8608001">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="GEOM_CalculateFPAINFOHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_CalculateFPAINFOHeaders.vi"/>
		<Item Name="GEOM_CalculateDETINFOHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_CalculateDETINFOHeaders.vi"/>
		<Item Name="GEOM_CalculateWCSHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_CalculateWCSHeaders.vi"/>
		<Item Name="PAN_run.vi" Type="VI" URL="../public/bin/PAN_run.vi"/>
		<Item Name="MNSN_Cmds.App.vi" Type="VI" URL="../Modules/MNSN/public/vis/MNSN_Cmds.App.vi"/>
		<Item Name="GEOM_CalculateNOAOHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_CalculateNOAOHeaders.vi"/>
		<Item Name="LOGIC_algors.vi" Type="VI" URL="../Modules/Logical/Algors/LOGIC_algors.vi"/>
		<Item Name="GFITS_Main.vi" Type="VI" URL="../Modules/GFITS/public/vis/GFITS_Main.vi"/>
		<Item Name="GUNSC_Main.vi" Type="VI" URL="../Modules/GUNSC/public/vis/GUNSC_Main.vi"/>
		<Item Name="TPNT_Main.vi" Type="VI" URL="../Modules/TPNT/public/vis/TPNT_Main.vi"/>
		<Item Name="TPNT_ExternalServer.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ExternalServer.vi"/>
		<Item Name="TPNT_TcpServer.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_TcpServer.vi"/>
		<Item Name="TPNT_API.vi" Type="VI" URL="../Modules/TPNT/public/vis/TPNT_API.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Destroy Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Destroy Semaphore.vi"/>
				<Item Name="Create Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Create Semaphore.vi"/>
				<Item Name="Validate Semaphore Size.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Validate Semaphore Size.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="Read Characters From File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Characters From File.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="Open File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Open File+.vi"/>
				<Item Name="Read File+ (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read File+ (string).vi"/>
				<Item Name="compatReadText.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatReadText.vi"/>
				<Item Name="Merge Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Merge Errors.vi"/>
				<Item Name="Close File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Close File+.vi"/>
				<Item Name="Find First Error.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find First Error.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="Preload Instrument.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/victl.llb/Preload Instrument.vi"/>
				<Item Name="viRef buffer.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/victl.llb/viRef buffer.vi"/>
				<Item Name="Get Instrument State.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/victl.llb/Get Instrument State.vi"/>
				<Item Name="Read Lines From File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Lines From File.vi"/>
				<Item Name="System Exec.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/system.llb/System Exec.vi"/>
				<Item Name="Open Config Data.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Open Config Data.vi"/>
				<Item Name="Config Data Open Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Open Reference.vi"/>
				<Item Name="Config Data Registry Functions.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Registry Functions.ctl"/>
				<Item Name="Config Data Registry.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Registry.vi"/>
				<Item Name="Config Data.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data.ctl"/>
				<Item Name="Config Data Section.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Section.ctl"/>
				<Item Name="Config Data Set File Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Set File Path.vi"/>
				<Item Name="Config Data Modify Functions.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Modify Functions.ctl"/>
				<Item Name="Config Data Modify.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Modify.vi"/>
				<Item Name="Add Quotes.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Add Quotes.vi"/>
				<Item Name="Info From Config Data.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Info From Config Data.vi"/>
				<Item Name="Config Data Read From File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Read From File.vi"/>
				<Item Name="Config Data Get File Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Get File Path.vi"/>
				<Item Name="String to Config Data.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/String to Config Data.vi"/>
				<Item Name="Invalid Config Data Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Invalid Config Data Reference.vi"/>
				<Item Name="Config Data Close Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Close Reference.vi"/>
				<Item Name="Get Section Names.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Get Section Names.vi"/>
				<Item Name="Get Key Names.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Get Key Names.vi"/>
				<Item Name="Read Key.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key.vi"/>
				<Item Name="Read Key (Boolean).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (Boolean).vi"/>
				<Item Name="Config Data Get Key Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Get Key Value.vi"/>
				<Item Name="Read Key (Double).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (Double).vi"/>
				<Item Name="Read Key (I32).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (I32).vi"/>
				<Item Name="Read Key (Path).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (Path).vi"/>
				<Item Name="Remove Quotes.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Remove Quotes.vi"/>
				<Item Name="Common Path to Specific Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Common Path to Specific Path.vi"/>
				<Item Name="Read Key (String).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (String).vi"/>
				<Item Name="Parse Stored String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Parse Stored String.vi"/>
				<Item Name="Get Next Character.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Get Next Character.vi"/>
				<Item Name="Read Key (U32).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Read Key (U32).vi"/>
				<Item Name="Close Config Data.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Close Config Data.vi"/>
				<Item Name="Config Data Write To File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data Write To File.vi"/>
				<Item Name="Config Data to String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data to String.vi"/>
				<Item Name="Write Key.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key.vi"/>
				<Item Name="Write Key (Boolean).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (Boolean).vi"/>
				<Item Name="Write Key (Double).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (Double).vi"/>
				<Item Name="Write Key (I32).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (I32).vi"/>
				<Item Name="Write Key (Path).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (Path).vi"/>
				<Item Name="Specific Path to Common Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Specific Path to Common Path.vi"/>
				<Item Name="Write Key (String).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (String).vi"/>
				<Item Name="Single to Double Backslash.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Single to Double Backslash.vi"/>
				<Item Name="Remove Unprintable Chars.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Remove Unprintable Chars.vi"/>
				<Item Name="Write Key (U32).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Write Key (U32).vi"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="Write File+ (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write File+ (string).vi"/>
				<Item Name="compatWriteText.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatWriteText.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="LV70DateRecToTimeStamp.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/LV70DateRecToTimeStamp.vi"/>
				<Item Name="LV70U32ToDateRec.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/LV70U32ToDateRec.vi"/>
				<Item Name="NI_Gmath.lvlib" Type="Library" URL="/&lt;vilib&gt;/gmath/NI_Gmath.lvlib"/>
				<Item Name="Semaphore RefNum" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore RefNum"/>
				<Item Name="Config Data RefNum" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Config Data RefNum"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="LV70TimeStampToDateRec.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/LV70TimeStampToDateRec.vi"/>
				<Item Name="Release Semaphore Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Release Semaphore Reference.vi"/>
				<Item Name="RemoveNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/RemoveNamedSemaphorePrefix.vi"/>
				<Item Name="GetNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/GetNamedSemaphorePrefix.vi"/>
				<Item Name="Semaphore Refnum Core.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore Refnum Core.ctl"/>
				<Item Name="Semaphore Name &amp; Ref DB.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore Name &amp; Ref DB.vi"/>
				<Item Name="Not A Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Not A Semaphore.vi"/>
				<Item Name="Semaphore Name &amp; Ref DB Action.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Semaphore Name &amp; Ref DB Action.ctl"/>
				<Item Name="Get Semaphore Status.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Get Semaphore Status.vi"/>
				<Item Name="Obtain Semaphore Reference.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Obtain Semaphore Reference.vi"/>
				<Item Name="AddNamedSemaphorePrefix.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/AddNamedSemaphorePrefix.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="Create Queue.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Create Queue.vi"/>
				<Item Name="Queue SRdB.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Queue SRdB.vi"/>
				<Item Name="Queue SRdB command.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Queue SRdB command.ctl"/>
				<Item Name="Queue RefNum" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Queue RefNum"/>
				<Item Name="Insert Queue Element.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Insert Queue Element.vi"/>
				<Item Name="Remove Queue Element.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Remove Queue Element.vi"/>
				<Item Name="Flush Queue.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Flush Queue.vi"/>
				<Item Name="Destroy Queue.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/queue.llb/Destroy Queue.vi"/>
				<Item Name="Remove Key.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/Remove Key.vi"/>
				<Item Name="Acquire Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Acquire Semaphore.vi"/>
				<Item Name="Release Semaphore.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/semaphor.llb/Release Semaphore.vi"/>
				<Item Name="Write Characters To File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Characters To File.vi"/>
				<Item Name="NI_SMTPEmail.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/SMTP/NI_SMTPEmail.lvlib"/>
				<Item Name="Beep.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/system.llb/Beep.vi"/>
				<Item Name="NI_AALBase.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/NI_AALBase.lvlib"/>
				<Item Name="NI_report.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Utility/NIReport.llb/NI_report.lvclass"/>
				<Item Name="NI_ReportGenerationCore.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/NIReport.llb/NI_ReportGenerationCore.lvlib"/>
				<Item Name="Get LV Class Default Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/LVClass/Get LV Class Default Value.vi"/>
				<Item Name="NI_HTML.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Utility/NIReport.llb/HTML/NI_HTML.lvclass"/>
				<Item Name="Write JPEG File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Write JPEG File.vi"/>
				<Item Name="Check Data Size.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Data Size.vi"/>
				<Item Name="Check Color Table Size.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Color Table Size.vi"/>
				<Item Name="Check Path.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check Path.vi"/>
				<Item Name="Directory of Top Level VI.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Directory of Top Level VI.vi"/>
				<Item Name="Check File Permissions.vi" Type="VI" URL="/&lt;vilib&gt;/picture/jpeg.llb/Check File Permissions.vi"/>
				<Item Name="imagedata.ctl" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/imagedata.ctl"/>
				<Item Name="Write PNG File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/png.llb/Write PNG File.vi"/>
			</Item>
			<Item Name="DatabaseHand_Create.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_Create.vi"/>
			<Item Name="DatabaseHan_Not_a_Refnum.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHan_Not_a_Refnum.vi"/>
			<Item Name="DatabaseHandler_Core.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHandler_Core.vi"/>
			<Item Name="CMN_modinfo.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_modinfo.vi"/>
			<Item Name="CMN_PassMsgToRunProc.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_PassMsgToRunProc.vi"/>
			<Item Name="DatabaseHand_Element_Set(error).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(error).vi"/>
			<Item Name="DatabaseHand_Element_Get(double).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(double).vi"/>
			<Item Name="CMN_KeyValToPairs.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_KeyValToPairs.vi"/>
			<Item Name="GEOM_CommandSwitch.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CommandSwitch.vi"/>
			<Item Name="CMN_toMdMgr.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toMdMgr.vi"/>
			<Item Name="GEOM_chk_override_format.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_chk_override_format.vi"/>
			<Item Name="GEOM_AmpsizeVariable2numbers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AmpsizeVariable2numbers.vi"/>
			<Item Name="Parse Arithmetic Expression.vi" Type="VI" URL="../Modules/Common/public/vis/Parse_Arithmetic_Expression.llb/Parse Arithmetic Expression.vi"/>
			<Item Name="Process Operators.vi" Type="VI" URL="../Modules/Common/public/vis/Parse_Arithmetic_Expression.llb/Process Operators.vi"/>
			<Item Name="Perform One Operation.vi" Type="VI" URL="../Modules/Common/public/vis/Parse_Arithmetic_Expression.llb/Perform One Operation.vi"/>
			<Item Name="GEOM_VAR_action.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_action.ctl"/>
			<Item Name="CMN_testpath.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_testpath.vi"/>
			<Item Name="CMN_PathParams2Path.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_PathParams2Path.vi"/>
			<Item Name="CMN_ConfigVals.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_ConfigVals.vi"/>
			<Item Name="CMN_Rel2AbsPath.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Rel2AbsPath.vi"/>
			<Item Name="CMN_WriteCfgToMem.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_WriteCfgToMem.vi"/>
			<Item Name="CMN_ReadCfgFromMem.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_ReadCfgFromMem.vi"/>
			<Item Name="GEOM_template2keys.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_template2keys.vi"/>
			<Item Name="GEOM_HeaderModule.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_HeaderModule.ctl"/>
			<Item Name="GEOM_GetHdrType.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetHdrType.vi"/>
			<Item Name="GEOM_VAR_HeaderModules.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_HeaderModules.vi"/>
			<Item Name="GEOM_Gbls.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Gbls.vi"/>
			<Item Name="GEOM_GeomMngr.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GeomMngr.vi"/>
			<Item Name="GEOM_Coords.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Coords.ctl"/>
			<Item Name="GEOM_FPAReadMode.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FPAReadMode.ctl"/>
			<Item Name="GEOM_Amplifier.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Amplifier.ctl"/>
			<Item Name="GEOM_AmpType.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AmpType.ctl"/>
			<Item Name="GEOM_ROI.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ROI.ctl"/>
			<Item Name="GEOM_ROICorrection.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ROICorrection.ctl"/>
			<Item Name="GEOM_VAR_FPAReadModes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_FPAReadModes.vi"/>
			<Item Name="GEOM_VAR_GeomModifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_GeomModifiers.vi"/>
			<Item Name="GEOM_AmpModifiers.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AmpModifiers.ctl"/>
			<Item Name="GEOM_Binning.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Binning.ctl"/>
			<Item Name="GEOM_Moifier.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Moifier.ctl"/>
			<Item Name="GEOM_SaveReqROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SaveReqROI.vi"/>
			<Item Name="GEOM_VAR_RequestedROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_RequestedROI.vi"/>
			<Item Name="GEOM_ReadFPA.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadFPA.vi"/>
			<Item Name="GEOM_FPADetectors.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FPADetectors.ctl"/>
			<Item Name="GEOM_ReadMode.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadMode.ctl"/>
			<Item Name="GEOM_ReadDetector.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadDetector.vi"/>
			<Item Name="GEOM_ReadDetAmps.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadDetAmps.vi"/>
			<Item Name="GEOM_SortAmplifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SortAmplifiers.vi"/>
			<Item Name="GEOM_ReadDetInfo.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadDetInfo.vi"/>
			<Item Name="GEOM_ReadType.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadType.vi"/>
			<Item Name="GEOM_GetValueFromInfo.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetValueFromInfo.vi"/>
			<Item Name="GEOM_Read_ReadModes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Read_ReadModes.vi"/>
			<Item Name="GEOM_UpdateDetInfo.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_UpdateDetInfo.vi"/>
			<Item Name="GEOM_SortDetectors.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SortDetectors.vi"/>
			<Item Name="GEOM_Read_FPAReadModes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Read_FPAReadModes.vi"/>
			<Item Name="GEOM_FPAamplifiersList.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FPAamplifiersList.vi"/>
			<Item Name="GEOM_FindReadMode.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FindReadMode.vi"/>
			<Item Name="GEOM_FPA2ReadMode.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FPA2ReadMode.vi"/>
			<Item Name="GEOM_ReadModifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadModifiers.vi"/>
			<Item Name="GEOM_CalcHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_CalcHeaders.vi"/>
			<Item Name="GEOM_ReadModeHeader.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadModeHeader.ctl"/>
			<Item Name="GEOM_VAR_BinCorrectedFPA.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_BinCorrectedFPA.vi"/>
			<Item Name="GEOM_CorrectedFPA.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrectedFPA.ctl"/>
			<Item Name="GEOM_VAR_OrigFPAReadModes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_OrigFPAReadModes.vi"/>
			<Item Name="GEOM_VAR_HeadersArray.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_HeadersArray.vi"/>
			<Item Name="GEOM_MoveHeaders2.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_MoveHeaders2.vi"/>
			<Item Name="GEOM_CorrectCoords.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrectCoords.vi"/>
			<Item Name="GEOM_CorrectByRotation.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrectByRotation.vi"/>
			<Item Name="GEOM_All2LLcoords.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_All2LLcoords.vi"/>
			<Item Name="GEOM_Size2Coords.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Size2Coords.vi"/>
			<Item Name="GEOM_TAMP2LL.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_TAMP2LL.vi"/>
			<Item Name="GEOM_Coords2Size.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Coords2Size.vi"/>
			<Item Name="GEOM_SortByCoords.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_SortByCoords.vi"/>
			<Item Name="GEOM_Sortby1coord.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_Sortby1coord.vi"/>
			<Item Name="GEOM_findAmplifierbyCoord.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_findAmplifierbyCoord.vi"/>
			<Item Name="GEOM_SortROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SortROIs.vi"/>
			<Item Name="GEOM_VAR_FPA.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_FPA.vi"/>
			<Item Name="GEOM_GetTotData.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetTotData.vi"/>
			<Item Name="GEOM_VAR_NonBinCorrectedFPA.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_NonBinCorrectedFPA.vi"/>
			<Item Name="GEOM_VAR_OrigFPA.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_OrigFPA.vi"/>
			<Item Name="GEOM_SetModifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetModifiers.vi"/>
			<Item Name="GEOM_CorrectAmplifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrectAmplifiers.vi"/>
			<Item Name="GEOM_CleanModifiers.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CleanModifiers.vi"/>
			<Item Name="GEOM_BinCoords.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_BinCoords.vi"/>
			<Item Name="GEOM_SetBinning.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetBinning.vi"/>
			<Item Name="GEOM_FPA2DetROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FPA2DetROI.vi"/>
			<Item Name="GEOM_CleanROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CleanROIs.vi"/>
			<Item Name="GEOM_SingleFPA2DetROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SingleFPA2DetROI.vi"/>
			<Item Name="GEOM_ROICorrections.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ROICorrections.vi"/>
			<Item Name="GEOM_RCSimUseCols.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_RCSimUseCols.vi"/>
			<Item Name="GEOM_TLL2AMP.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_TLL2AMP.vi"/>
			<Item Name="GEOM_RCSimAllCols.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_RCSimAllCols.vi"/>
			<Item Name="GEOM_AddROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AddROIs.vi"/>
			<Item Name="GEOM_AbsROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AbsROI.vi"/>
			<Item Name="GEOM_findReadMode.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_findReadMode.vi"/>
			<Item Name="CMN_Str2Arr.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Str2Arr.vi"/>
			<Item Name="GEOM_GetReadModes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetReadModes.vi"/>
			<Item Name="CMN_Arr2Str.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Arr2Str.vi"/>
			<Item Name="GEOM_SetNampsxy.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetNampsxy.vi"/>
			<Item Name="GEOM_GetHeaders.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetHeaders.vi"/>
			<Item Name="CMN_2DArr2Str.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_2DArr2Str.vi"/>
			<Item Name="GEOM_Geom2Str.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Geom2Str.vi"/>
			<Item Name="BUFMGR_toBufmgr.vi" Type="VI" URL="../Modules/buffermgr/BUFMGR_toBufmgr.vi"/>
			<Item Name="PAN_PostCommand.vi" Type="VI" URL="../private/vis/PAN_PostCommand.vi"/>
			<Item Name="PAN_actionsSTruct.ctl" Type="VI" URL="../private/vis/PAN_actionsSTruct.ctl"/>
			<Item Name="PAN_SetTimeout.vi" Type="VI" URL="../private/vis/PAN_SetTimeout.vi"/>
			<Item Name="DHE_RunProc.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_RunProc.vi"/>
			<Item Name="PAN_GetCmdQ.vi" Type="VI" URL="../public/vis/PAN_GetCmdQ.vi"/>
			<Item Name="DHE_GetAsyncN.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetAsyncN.vi"/>
			<Item Name="PAN_ActionsMgr.vi" Type="VI" URL="../private/vis/PAN_ActionsMgr.vi"/>
			<Item Name="PAN_GetAsyncN.vi" Type="VI" URL="../public/vis/PAN_GetAsyncN.vi"/>
			<Item Name="PAN_FindComs.vi" Type="VI" URL="../private/vis/PAN_FindComs.vi"/>
			<Item Name="CMN_CommandSwitch.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_CommandSwitch.vi"/>
			<Item Name="PAN_GetCmdLineArgs.vi" Type="VI" URL="../private/vis/PAN_GetCmdLineArgs.vi"/>
			<Item Name="libmem.so.20" Type="Document" URL="../Modules/Common/Clib/lib/libmem.so.20"/>
			<Item Name="CMN_toBufmgr.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toBufmgr.vi"/>
			<Item Name="CMN_BadParamMsg.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_BadParamMsg.vi"/>
			<Item Name="CMN_Write_Characterss_To_File.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Write_Characterss_To_File.vi"/>
			<Item Name="GUNSC_toUnsc.vi" Type="VI" URL="../Modules/GUNSC/public/vis/GUNSC_toUnsc.vi"/>
			<Item Name="PAN_SetGlobalPaths.vi" Type="VI" URL="../private/vis/PAN_SetGlobalPaths.vi"/>
			<Item Name="DHE.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE.vi"/>
			<Item Name="DHE_GetLogFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetLogFile.vi"/>
			<Item Name="DHE_GBLDriverVariables.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GBLDriverVariables.vi"/>
			<Item Name="CMN_TimingInfo.ctl" Type="VI" URL="../Modules/Common/public/vis/CMN_TimingInfo.ctl"/>
			<Item Name="DHE_GetLog.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetLog.vi"/>
			<Item Name="DHE_fdMgr.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_fdMgr.vi"/>
			<Item Name="DHE_Device.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_Device.ctl"/>
			<Item Name="DHE_Getfd.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Getfd.vi"/>
			<Item Name="DHE_SetActiveDevice.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetActiveDevice.vi"/>
			<Item Name="CMN_ToServerLogQ.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_ToServerLogQ.vi"/>
			<Item Name="DHE_Macro.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Macro.vi"/>
			<Item Name="DHE_GetMacroDir.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetMacroDir.vi"/>
			<Item Name="DHE_ReplaceMacroArgs.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ReplaceMacroArgs.vi"/>
			<Item Name="DHE_ParseCommand.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ParseCommand.vi"/>
			<Item Name="CMN_GetPhysUnit.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetPhysUnit.vi"/>
			<Item Name="DHE_GetDBS.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetDBS.vi"/>
			<Item Name="DHE_GetImgState.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetImgState.vi"/>
			<Item Name="DHE_CheckCommandTmout.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_CheckCommandTmout.vi"/>
			<Item Name="DHE_GetFromDBS.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetFromDBS.vi"/>
			<Item Name="DHE_GetControllerStatus.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetControllerStatus.vi"/>
			<Item Name="CMN_ContStatus.ctl" Type="VI" URL="../Modules/Common/public/vis/CMN_ContStatus.ctl"/>
			<Item Name="DatabaseHand_FindSubGroup.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_FindSubGroup.vi"/>
			<Item Name="DatabaseHand_findGroup.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_findGroup.vi"/>
			<Item Name="DHE_send_custom.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_send_custom.vi"/>
			<Item Name="DHE_custom.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_custom.vi"/>
			<Item Name="DHE_GetModRef.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetModRef.vi"/>
			<Item Name="DHE_GetModuleName.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetModuleName.vi"/>
			<Item Name="DHE_CheckBusy.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_CheckBusy.vi"/>
			<Item Name="DHE_GetAvailableCmds.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetAvailableCmds.vi"/>
			<Item Name="CMN_SeparateField.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SeparateField.vi"/>
			<Item Name="DHE_Settings.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Settings.vi"/>
			<Item Name="DHE_GetImageParams.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetImageParams.vi"/>
			<Item Name="DHE_Img.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_Img.ctl"/>
			<Item Name="CMN_GetPhysPath.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetPhysPath.vi"/>
			<Item Name="DHE_BuildRootname.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_BuildRootname.vi"/>
			<Item Name="DHE_SetGeometry.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetGeometry.vi"/>
			<Item Name="DHE_GetReadMode.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetReadMode.vi"/>
			<Item Name="DHE_Getbpp.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_Getbpp.vi"/>
			<Item Name="DHE_GetDetXYSize.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetDetXYSize.vi"/>
			<Item Name="GEOM_CallGeometry.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_CallGeometry.vi"/>
			<Item Name="GEOM_GetFPA.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetFPA.vi"/>
			<Item Name="DHE_dimensions.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_dimensions.vi"/>
			<Item Name="DHE_set_geom.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_geom.vi"/>
			<Item Name="DHE_calcbuffers.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_calcbuffers.vi"/>
			<Item Name="DHE_Getrawbuffersize.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_Getrawbuffersize.vi"/>
			<Item Name="CMN_Buffers.ctl" Type="VI" URL="../Modules/Common/public/vis/CMN_Buffers.ctl"/>
			<Item Name="CMN_SetBuffers.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SetBuffers.vi"/>
			<Item Name="DHE_SetTiming.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetTiming.vi"/>
			<Item Name="DHE_GetTimingInfo.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetTimingInfo.vi"/>
			<Item Name="DHE_GetSeqOverhead.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetSeqOverhead.vi"/>
			<Item Name="DHE_VarAction.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_VarAction.ctl"/>
			<Item Name="DHE_VAR_SeqsOverhead.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_SeqsOverhead.vi"/>
			<Item Name="DHE_GetReadOverhead.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetReadOverhead.vi"/>
			<Item Name="DHE_GetExpOverhead.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetExpOverhead.vi"/>
			<Item Name="DHE_VAR_ExpOverhead.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ExpOverhead.vi"/>
			<Item Name="DHE_GetActiveDevice.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetActiveDevice.vi"/>
			<Item Name="CMN_CalcDetReadTime.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_CalcDetReadTime.vi"/>
			<Item Name="DHE_get_frame_time.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_get_frame_time.vi"/>
			<Item Name="DHE_SetTimingInfo.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetTimingInfo.vi"/>
			<Item Name="DHE_SetSizeVals.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetSizeVals.vi"/>
			<Item Name="DHE_SetControllerParams.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetControllerParams.vi"/>
			<Item Name="DHE_SetImageParams.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetImageParams.vi"/>
			<Item Name="DHE_GetObsParams.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetObsParams.vi"/>
			<Item Name="DHE_Obs.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_Obs.ctl"/>
			<Item Name="DHE_ImtypeActions.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ImtypeActions.vi"/>
			<Item Name="DHE_ParseAutoshutterCmd.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ParseAutoshutterCmd.vi"/>
			<Item Name="DHE_autoshutter.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_autoshutter.vi"/>
			<Item Name="DHE_set_exp_time.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_exp_time.vi"/>
			<Item Name="DHE_GetMinExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetMinExpTime.vi"/>
			<Item Name="DHE_VAR_MinExpTime.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_MinExpTime.vi"/>
			<Item Name="DHE_set_nimages.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_nimages.vi"/>
			<Item Name="DHE_SetROI.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetROI.vi"/>
			<Item Name="DHE_set_roi.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_roi.vi"/>
			<Item Name="DHE_set_ncoadds.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_ncoadds.vi"/>
			<Item Name="DHE_autoSample.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_autoSample.vi"/>
			<Item Name="DHE_SetObsParams.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetObsParams.vi"/>
			<Item Name="DHE_SetSampling.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetSampling.vi"/>
			<Item Name="DHE_GetControllerConfig.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetControllerConfig.vi"/>
			<Item Name="DHE_set_sampling.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_sampling.vi"/>
			<Item Name="DHE_set_nsur.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_nsur.vi"/>
			<Item Name="DHE_set_fsamples.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_fsamples.vi"/>
			<Item Name="DHE_idle.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_idle.vi"/>
			<Item Name="GEOM_GetModifiers.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetModifiers.vi"/>
			<Item Name="DHE_SetReadMode.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetReadMode.vi"/>
			<Item Name="DHE_GetConfigFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetConfigFile.vi"/>
			<Item Name="DHE_ReadModes.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ReadModes.vi"/>
			<Item Name="DHE_read_mode.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_read_mode.vi"/>
			<Item Name="DHE_SetReadModeVar.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetReadModeVar.vi"/>
			<Item Name="DHE_SetFPASizeTotVals.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetFPASizeTotVals.vi"/>
			<Item Name="DHE_SetDetXYSize.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetDetXYSize.vi"/>
			<Item Name="DHE_SetBinning.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetBinning.vi"/>
			<Item Name="DHE_binning.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_binning.vi"/>
			<Item Name="DHE_SetBinValues.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetBinValues.vi"/>
			<Item Name="DHE_artif_data.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_artif_data.vi"/>
			<Item Name="DHE_GetArtifPatterns.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetArtifPatterns.vi"/>
			<Item Name="DHE_VAR_ArtifPatterns.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ArtifPatterns.vi"/>
			<Item Name="DHE_set_artifpattern.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_artifpattern.vi"/>
			<Item Name="DHE_autoclear.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_autoclear.vi"/>
			<Item Name="DHE_power.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_power.vi"/>
			<Item Name="DHE_set_gain.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_gain.vi"/>
			<Item Name="DHE_set_speed.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_speed.vi"/>
			<Item Name="DHE_move_shutter.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_move_shutter.vi"/>
			<Item Name="DHE_set_shutter_type.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_shutter_type.vi"/>
			<Item Name="DHE_InitGeometry.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_InitGeometry.vi"/>
			<Item Name="DHE_GetFPAname.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetFPAname.vi"/>
			<Item Name="DHE_GetModifiers.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetModifiers.vi"/>
			<Item Name="DHE_SetControllerConfig.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetControllerConfig.vi"/>
			<Item Name="DHE_set_bpp.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_bpp.vi"/>
			<Item Name="DHE_Get.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Get.vi"/>
			<Item Name="GEOM_GetROI.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetROI.vi"/>
			<Item Name="DHE_GetProgress.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetProgress.vi"/>
			<Item Name="DHE_GetElapsedExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetElapsedExpTime.vi"/>
			<Item Name="DHE_VAR_ElapsedExpTime.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ElapsedExpTime.vi"/>
			<Item Name="DHE_GetReadProgress.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetReadProgress.vi"/>
			<Item Name="DHE_VAR_ReadProgress.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ReadProgress.vi"/>
			<Item Name="DHE_GetGeometry.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetGeometry.vi"/>
			<Item Name="DHE_GetModifiersInfo.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetModifiersInfo.vi"/>
			<Item Name="GEOM_GetAvailableReadModes.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetAvailableReadModes.vi"/>
			<Item Name="DHE_get_samplings.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_get_samplings.vi"/>
			<Item Name="CMN_Struct2Str.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Struct2Str.vi"/>
			<Item Name="DHE_get_controller_info.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_get_controller_info.vi"/>
			<Item Name="DHE_CalcExptime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_CalcExptime.vi"/>
			<Item Name="DHE_CalcMinExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_CalcMinExpTime.vi"/>
			<Item Name="DHE_GetManualMinExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetManualMinExpTime.vi"/>
			<Item Name="DHE_VAR_ManualMinExpTime.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ManualMinExpTime.vi"/>
			<Item Name="DHE_SetMinExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetMinExpTime.vi"/>
			<Item Name="DHE_GetStatus.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetStatus.vi"/>
			<Item Name="CMN_Efficiency.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Efficiency.vi"/>
			<Item Name="CMN_TimeStamp2Seconds.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_TimeStamp2Seconds.vi"/>
			<Item Name="DHE_GetInitMacro.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetInitMacro.vi"/>
			<Item Name="GEOM_GetFPAINFO.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetFPAINFO.vi"/>
			<Item Name="GEOM_GetDETNFO.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetDETNFO.vi"/>
			<Item Name="DHE_TP.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_TP.vi"/>
			<Item Name="DHE_GetTPconfig.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetTPconfig.vi"/>
			<Item Name="DHE_ReadTPs.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ReadTPs.vi"/>
			<Item Name="DHE_TP.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_TP.ctl"/>
			<Item Name="DHE_TPLimits.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_TPLimits.ctl"/>
			<Item Name="DHE_GetCurrStatus.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetCurrStatus.vi"/>
			<Item Name="DHE_ReadTP.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ReadTP.vi"/>
			<Item Name="DHE_read_tp.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_read_tp.vi"/>
			<Item Name="CMN_Algors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Algors.vi"/>
			<Item Name="CMN_LabMathParser.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_LabMathParser.vi"/>
			<Item Name="DHE_GetHdrType.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetHdrType.vi"/>
			<Item Name="DHE_SendAsync.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SendAsync.vi"/>
			<Item Name="CMN_toTPNT.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toTPNT.vi"/>
			<Item Name="DHE_TPGetLastValue.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_TPGetLastValue.vi"/>
			<Item Name="DHE_TPs2DArrr.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_TPs2DArrr.vi"/>
			<Item Name="DHE_WriteTP.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_WriteTP.vi"/>
			<Item Name="DHE_write_tp.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_write_tp.vi"/>
			<Item Name="DHE_StartTPNT.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_StartTPNT.vi"/>
			<Item Name="DHE_GetAlgorsFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetAlgorsFile.vi"/>
			<Item Name="DHE_Utils.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Utils.vi"/>
			<Item Name="DHE_SetControllerStatus.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetControllerStatus.vi"/>
			<Item Name="DHE_clear_array.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_clear_array.vi"/>
			<Item Name="DHE_abort_all.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_abort_all.vi"/>
			<Item Name="CMN_SetReadoutError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SetReadoutError.vi"/>
			<Item Name="DHE_SetImgState.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetImgState.vi"/>
			<Item Name="DHE_stop_sequence.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_stop_sequence.vi"/>
			<Item Name="DHE_stop_exposure.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_stop_exposure.vi"/>
			<Item Name="DHE_Expose.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Expose.vi"/>
			<Item Name="CMN_CheckDisk.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_CheckDisk.vi"/>
			<Item Name="DHE_GoExpose.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GoExpose.vi"/>
			<Item Name="DHE_SetBufError.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetBufError.vi"/>
			<Item Name="DHE_SetPrePostNumFrames.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetPrePostNumFrames.vi"/>
			<Item Name="DHE_SetElapsedExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetElapsedExpTime.vi"/>
			<Item Name="DHE_SetReadProgress.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetReadProgress.vi"/>
			<Item Name="CMN_SetImprocError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SetImprocError.vi"/>
			<Item Name="CMN_SetUnscError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SetUnscError.vi"/>
			<Item Name="DHE_clear_progress_buf.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_clear_progress_buf.vi"/>
			<Item Name="DHE_IRExpose.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_IRExpose.vi"/>
			<Item Name="DHE_pause_exposure.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_pause_exposure.vi"/>
			<Item Name="DHE_resume_exposure.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_resume_exposure.vi"/>
			<Item Name="DHE_reset_controller.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_reset_controller.vi"/>
			<Item Name="DHE_close_driver.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_close_driver.vi"/>
			<Item Name="DHE_SetStatus.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetStatus.vi"/>
			<Item Name="DHE_SetDBS.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetDBS.vi"/>
			<Item Name="DHE_SetLogFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetLogFile.vi"/>
			<Item Name="DHE_SetLog.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetLog.vi"/>
			<Item Name="DHE_SetConfigFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetConfigFile.vi"/>
			<Item Name="DHE_SetInstLibPath.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetInstLibPath.vi"/>
			<Item Name="DHE_VAR_InstLibPath.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_InstLibPath.vi"/>
			<Item Name="DHE_SetMacroDir.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetMacroDir.vi"/>
			<Item Name="DHE_SetAlgorsFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetAlgorsFile.vi"/>
			<Item Name="DHE_SetTPconfig.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetTPconfig.vi"/>
			<Item Name="DHE_ReadTimingFromFile.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ReadTimingFromFile.vi"/>
			<Item Name="DHE_SetSeqsOverhead.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetSeqsOverhead.vi"/>
			<Item Name="DHE_SetManualMinExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetManualMinExpTime.vi"/>
			<Item Name="DHE_SetExpOverhead.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetExpOverhead.vi"/>
			<Item Name="DHE_SetModifiers.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetModifiers.vi"/>
			<Item Name="DHE_SetFPAname.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetFPAname.vi"/>
			<Item Name="DHE_SyncImage.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SyncImage.vi"/>
			<Item Name="DHE_SetCommandToExecute.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetCommandToExecute.vi"/>
			<Item Name="DHE_LoadDHEModule.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_LoadDHEModule.vi"/>
			<Item Name="DHE_SetModuleName.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetModuleName.vi"/>
			<Item Name="DHE_SetDACsFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetDACsFile.vi"/>
			<Item Name="DHE_VAR_DACsFile.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_DACsFile.vi"/>
			<Item Name="DHE_SetREGsFile.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetREGsFile.vi"/>
			<Item Name="DHE_VAR_REGsFile.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_REGsFile.vi"/>
			<Item Name="DHE_SetArtifPatterns.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetArtifPatterns.vi"/>
			<Item Name="DHE_version.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_version.vi"/>
			<Item Name="CMN_DescribeDriverErrors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_DescribeDriverErrors.vi"/>
			<Item Name="PAN_StartCom.vi" Type="VI" URL="../private/vis/PAN_StartCom.vi"/>
			<Item Name="CMN_toGRTD.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toGRTD.vi"/>
			<Item Name="CMN_addstatusmessagevi.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_addstatusmessagevi.vi"/>
			<Item Name="CMN_FVAR_stop_sequence.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_stop_sequence.vi"/>
			<Item Name="CMN_FVAR_images_read_done.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_images_read_done.vi"/>
			<Item Name="PAN_ToLog.vi" Type="VI" URL="../private/vis/PAN_ToLog.vi"/>
			<Item Name="DHE_SetInitMacro.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetInitMacro.vi"/>
			<Item Name="CMN_toUnsc.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toUnsc.vi"/>
			<Item Name="CMN_FVAR_frame_being_read.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_frame_being_read.vi"/>
			<Item Name="PAN_Shutdown.vi" Type="VI" URL="../private/vis/PAN_Shutdown.vi"/>
			<Item Name="GEOM_GetINFOKeyword.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetINFOKeyword.vi"/>
			<Item Name="CMN_ReadGblsFromFile.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_ReadGblsFromFile.vi"/>
			<Item Name="GEOM_API.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_API.vi"/>
			<Item Name="GEOM_Adjust_Sizes.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Adjust_Sizes.vi"/>
			<Item Name="GEOM_BinRemainders.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_BinRemainders.vi"/>
			<Item Name="BUFMGR_API.vi" Type="VI" URL="../Modules/buffermgr/BUFMGR_API.vi"/>
			<Item Name="PAN_GetMods.vi" Type="VI" URL="../private/vis/PAN_GetMods.vi"/>
			<Item Name="PAN_Translate.vi" Type="VI" URL="../private/vis/PAN_Translate.vi"/>
			<Item Name="PAN_ModCaller.vi" Type="VI" URL="../private/vis/PAN_ModCaller.vi"/>
			<Item Name="PAN_RunMacro.vi" Type="VI" URL="../private/vis/PAN_RunMacro.vi"/>
			<Item Name="PAN_Send_InternalCmd.vi" Type="VI" URL="../private/vis/PAN_Send_InternalCmd.vi"/>
			<Item Name="PAN_cmdStruct.ctl" Type="VI" URL="../private/vis/PAN_cmdStruct.ctl"/>
			<Item Name="CMN_TrimStringWhitespace.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_TrimStringWhitespace.vi"/>
			<Item Name="DatabaseHand_Group_Get(string).vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_Group_Get(string).vi"/>
			<Item Name="PAN_MacroMgr.vi" Type="VI" URL="../private/vis/PAN_MacroMgr.vi"/>
			<Item Name="PAN_MacroReadCmds.vi" Type="VI" URL="../private/vis/PAN_MacroReadCmds.vi"/>
			<Item Name="PAN_MacroCheckPath.vi" Type="VI" URL="../private/vis/PAN_MacroCheckPath.vi"/>
			<Item Name="PAN_MacroChange.vi" Type="VI" URL="../private/vis/PAN_MacroChange.vi"/>
			<Item Name="CMN_GetArg.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetArg.vi"/>
			<Item Name="PAN_ParseForCmdSet.vi" Type="VI" URL="../private/vis/PAN_ParseForCmdSet.vi"/>
			<Item Name="PAN_CheckSave.vi" Type="VI" URL="../private/vis/PAN_CheckSave.vi"/>
			<Item Name="PAN_MacroCmd.vi" Type="VI" URL="../private/vis/PAN_MacroCmd.vi"/>
			<Item Name="PAN_DBS.vi" Type="VI" URL="../private/vis/PAN_DBS.vi"/>
			<Item Name="DatabaseHand_Group_GetAll.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Group_GetAll.vi"/>
			<Item Name="DHE_ChkMacroPath.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_ChkMacroPath.vi"/>
			<Item Name="DHE_is_unsc_present.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_is_unsc_present.vi"/>
			<Item Name="CMN_SetMaxElePerBuffer.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SetMaxElePerBuffer.vi"/>
			<Item Name="DHE_set_roi_transf.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_set_roi_transf.vi"/>
			<Item Name="DHE_asyncvarsMgr.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_asyncvarsMgr.vi"/>
			<Item Name="DHE_asyncvarStruct.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_asyncvarStruct.ctl"/>
			<Item Name="DHE_get_link_info.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_get_link_info.vi"/>
			<Item Name="DHE_UpdateTPNTHdr.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_UpdateTPNTHdr.vi"/>
			<Item Name="DHE_reset_pci.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_reset_pci.vi"/>
			<Item Name="PAN_parseCmds.vi" Type="VI" URL="../private/vis/PAN_parseCmds.vi"/>
			<Item Name="PAN_sendSchedCmd.vi" Type="VI" URL="../private/vis/PAN_sendSchedCmd.vi"/>
			<Item Name="PAN_CreateDir.vi" Type="VI" URL="../private/vis/PAN_CreateDir.vi"/>
			<Item Name="PAN_Scheduler_action.ctl" Type="VI" URL="../private/vis/PAN_Scheduler_action.ctl"/>
			<Item Name="PAN_SchedulerMgr.vi" Type="VI" URL="../private/vis/PAN_SchedulerMgr.vi"/>
			<Item Name="PAN_scheduler_element.ctl" Type="VI" URL="../private/vis/PAN_scheduler_element.ctl"/>
			<Item Name="PAN_extract_schedule.vi" Type="VI" URL="../private/vis/PAN_extract_schedule.vi"/>
			<Item Name="PAN_ModsMgr.vi" Type="VI" URL="../private/vis/PAN_ModsMgr.vi"/>
			<Item Name="PAN_ChkResp.vi" Type="VI" URL="../private/vis/PAN_ChkResp.vi"/>
			<Item Name="CMN_FVAR_stop_coadd.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_stop_coadd.vi"/>
			<Item Name="CMN_FVAR_coadds_read_done.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_coadds_read_done.vi"/>
			<Item Name="DatabaseHand_Element_Get(string).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(string).vi"/>
			<Item Name="DatabaseHand_FindSubElement.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_FindSubElement.vi"/>
			<Item Name="DatabaseHand_Element.ctl" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element.ctl"/>
			<Item Name="DatabaseHan_DatabaseElement.ctl" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHan_DatabaseElement.ctl"/>
			<Item Name="DatabaseHand_Function.ctl" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Function.ctl"/>
			<Item Name="Database_Refnum.ctl" Type="VI" URL="../Modules/DBSHAND/private/vis/Database_Refnum.ctl"/>
			<Item Name="DatabaseHand_findSubEle.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_findSubEle.vi"/>
			<Item Name="DatabaseHand_GetSubElement.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_GetSubElement.vi"/>
			<Item Name="DatabaseHand_Element_Get(U16).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(U16).vi"/>
			<Item Name="DatabaseHand_Element_Get(I16).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(I16).vi"/>
			<Item Name="DatabaseHand_Element_Get(U32).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(U32).vi"/>
			<Item Name="DatabaseHand_Element_Get(I32).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(I32).vi"/>
			<Item Name="DatabaseHand_Element_Get(U8).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(U8).vi"/>
			<Item Name="DatabaseHand_Element_Get(I8).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(I8).vi"/>
			<Item Name="DatabaseHand_Element_Get(single).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(single).vi"/>
			<Item Name="DatabaseHand_Element_Get(extended).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(extended).vi"/>
			<Item Name="DatabaseHand_Element_Get(2DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(2DArr).vi"/>
			<Item Name="DatabaseHand_Element_Get(1DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(1DArr).vi"/>
			<Item Name="DatabaseHand_Element_Get(path).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(path).vi"/>
			<Item Name="DatabaseHand_Element_Get(error).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(error).vi"/>
			<Item Name="CMN_testmessage.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_testmessage.vi"/>
			<Item Name="DatabaseHand_SetSubElement.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_SetSubElement.vi"/>
			<Item Name="DatabaseHand_Element_Set(U16).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(U16).vi"/>
			<Item Name="DatabaseHand_Element_Set(U32).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(U32).vi"/>
			<Item Name="DatabaseHand_Element_Set(I32).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(I32).vi"/>
			<Item Name="DatabaseHand_Element_Set(U8).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(U8).vi"/>
			<Item Name="DatabaseHand_Element_Set(I8).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(I8).vi"/>
			<Item Name="DatabaseHand_Element_Set(single).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(single).vi"/>
			<Item Name="DatabaseHand_Element_Set(double).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(double).vi"/>
			<Item Name="DatabaseHand_Element_Set(extended).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(extended).vi"/>
			<Item Name="DatabaseHand_Element_Set(I16).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(I16).vi"/>
			<Item Name="DatabaseHand_Element_Set(2DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(2DArr).vi"/>
			<Item Name="DatabaseHand_Element_Set(1DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(1DArr).vi"/>
			<Item Name="DatabaseHand_Element_Set(path).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(path).vi"/>
			<Item Name="DHE_Setbpp.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_Setbpp.vi"/>
			<Item Name="CMN_Read_Lines_From_File.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Read_Lines_From_File.vi"/>
			<Item Name="GEOM_VAR_RequestedROIsArray.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_RequestedROIsArray.vi"/>
			<Item Name="GEOM_FindMaximunAmountOfAlignedAmps.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_FindMaximunAmountOfAlignedAmps.vi"/>
			<Item Name="GEOM_SortROILevels.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SortROILevels.vi"/>
			<Item Name="GEOM_CorrXcoord.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrXcoord.vi"/>
			<Item Name="GEOM_CorrYcoord.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrYcoord.vi"/>
			<Item Name="GEOM_MergeROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_MergeROIs.vi"/>
			<Item Name="GEOM_CheckMixedROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CheckMixedROIs.vi"/>
			<Item Name="GEOM_SetupROIs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetupROIs.vi"/>
			<Item Name="GEOM_CheckFullROI.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CheckFullROI.vi"/>
			<Item Name="GEOM_ROIArr2Str.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ROIArr2Str.vi"/>
			<Item Name="CMN_Read_Characters_From_File.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Read_Characters_From_File.vi"/>
			<Item Name="DHE_set_expCode.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_expCode.vi"/>
			<Item Name="CMN_ReplaceName.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_ReplaceName.vi"/>
			<Item Name="PAN_inspectName.vi" Type="VI" URL="../private/vis/PAN_inspectName.vi"/>
			<Item Name="PAN_FindCmdSetPath.vi" Type="VI" URL="../private/vis/PAN_FindCmdSetPath.vi"/>
			<Item Name="GEOM_GetCmdLineArgs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetCmdLineArgs.vi"/>
			<Item Name="DHE_InspectName.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_InspectName.vi"/>
			<Item Name="DHE_get_pixel_time.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_get_pixel_time.vi"/>
			<Item Name="DHE_set_soft_read_time.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_soft_read_time.vi"/>
			<Item Name="DHE_ChangeRawBpp.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ChangeRawBpp.vi"/>
			<Item Name="DHE_set_pixtime.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_pixtime.vi"/>
			<Item Name="DHE_SetAsyncExp.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetAsyncExp.vi"/>
			<Item Name="DHE_VAR_AsyncExpMsg.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_AsyncExpMsg.vi"/>
			<Item Name="DHE_SetAppendID.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetAppendID.vi"/>
			<Item Name="DHE_VAR_AppendID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_AppendID.vi"/>
			<Item Name="DHE_GetAsyncExp.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetAsyncExp.vi"/>
			<Item Name="DHE_GetAppendID.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetAppendID.vi"/>
			<Item Name="GEOM_GetStrAmps.vi" Type="VI" URL="../Modules/GEOM/public/vis/GEOM_GetStrAmps.vi"/>
			<Item Name="GEOM_Amp2Str.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Amp2Str.vi"/>
			<Item Name="lvanlys.so" Type="Document" URL="/usr/local/natinst/LabVIEW-8.6/resource/lvanlys.so"/>
			<Item Name="DHE_ResetexposureID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ResetexposureID.vi"/>
			<Item Name="DHE_ResetobsID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ResetobsID.vi"/>
			<Item Name="DHE_SetModRef.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetModRef.vi"/>
			<Item Name="PAN_RunRespSet.vi" Type="VI" URL="../private/vis/PAN_RunRespSet.vi"/>
			<Item Name="CMN_KeyVal2Array.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_KeyVal2Array.vi"/>
			<Item Name="CMN_CombineErrors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_CombineErrors.vi"/>
			<Item Name="CMN_GetControllerStatus.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetControllerStatus.vi"/>
			<Item Name="CMN_GetBuffers.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetBuffers.vi"/>
			<Item Name="CMN_FVAR_images_being_taken.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_images_being_taken.vi"/>
			<Item Name="CMN_FVAR_images_to_read.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_images_to_read.vi"/>
			<Item Name="CMN_GetReadoutError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetReadoutError.vi"/>
			<Item Name="CMN_FVAR_images_status.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_images_status.vi"/>
			<Item Name="CMN_toFits.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toFits.vi"/>
			<Item Name="toFramemgr.vi" Type="VI" URL="../Modules/buffermgr/framesMgr.llb/toFramemgr.vi"/>
			<Item Name="CMN_buildImageName.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_buildImageName.vi"/>
			<Item Name="DatabaseHand_RefNum.ctl" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_RefNum.ctl"/>
			<Item Name="DatabaseHand_Get.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_Get.vi"/>
			<Item Name="DatabaseHand_Element_Get(bool).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(bool).vi"/>
			<Item Name="CMN_FVAR_action.ctl" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_action.ctl"/>
			<Item Name="DatabaseHand_Set.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_Set.vi"/>
			<Item Name="DatabaseHand_Element_Set(string).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(string).vi"/>
			<Item Name="DatabaseHand_Element_Set(bool).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(bool).vi"/>
			<Item Name="DHE_template2keys.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_template2keys.vi"/>
			<Item Name="PAN_FindCmdSet3.vi" Type="VI" URL="../private/vis/PAN_FindCmdSet3.vi"/>
			<Item Name="PAN_RunCmdSet.vi" Type="VI" URL="../private/vis/PAN_RunCmdSet.vi"/>
			<Item Name="DHE_DO_INIT.vi" Type="VI" URL="../Modules/MNSN/private/vis/DHE_DO_INIT.vi"/>
			<Item Name="MNSN_SetInitVals.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SetInitVals.vi"/>
			<Item Name="DHE_GetCommandToExecute.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetCommandToExecute.vi"/>
			<Item Name="DHE_SetPixReadTimeAlgors.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetPixReadTimeAlgors.vi"/>
			<Item Name="DHE_VAR_PixReadAlgor.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_PixReadAlgor.vi"/>
			<Item Name="DHE_SetPixWriteTimeAlgors.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetPixWriteTimeAlgors.vi"/>
			<Item Name="DHE_VAR_PixWriteAlgor.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_PixWriteAlgor.vi"/>
			<Item Name="MNSN_INIT.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_INIT.vi"/>
			<Item Name="MNSN_SelectLink.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SelectLink.vi"/>
			<Item Name="DHE_DRV_LibInit.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_LibInit.vi"/>
			<Item Name="libarcDHE.so" Type="Document" URL="../Modules/MNSN/private/c/lib/libarcDHE.so"/>
			<Item Name="MNSN_GetSeqFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetSeqFile.vi"/>
			<Item Name="MNSN_SequencerPreProc.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SequencerPreProc.vi"/>
			<Item Name="MNSN_HWCFGMngr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFGMngr.vi"/>
			<Item Name="MNSN_HWCFG_ReadFromFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_ReadFromFile.vi"/>
			<Item Name="MNSN_HWCFG_Read1LineFromFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_Read1LineFromFile.vi"/>
			<Item Name="MNSN_AttField.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_AttField.ctl"/>
			<Item Name="MNSN_BoardStruct.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_BoardStruct.ctl"/>
			<Item Name="MNSN_BoardsMngr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_BoardsMngr.vi"/>
			<Item Name="MNSN_Numb2Dec.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Numb2Dec.vi"/>
			<Item Name="MNSN_HWCFG_Find.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_Find.vi"/>
			<Item Name="MNSN_ParseHWRequest.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ParseHWRequest.vi"/>
			<Item Name="MNSN_HWCFH_FormatLine2Str.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFH_FormatLine2Str.vi"/>
			<Item Name="MNSN_GenerateATTadresses.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GenerateATTadresses.vi"/>
			<Item Name="CMN_Str2Decimal.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Str2Decimal.vi"/>
			<Item Name="MNSN_att2dacparams.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_att2dacparams.vi"/>
			<Item Name="MNSN_StripMemAddress.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_StripMemAddress.vi"/>
			<Item Name="MNSN_HWCFG_CheckRange.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_CheckRange.vi"/>
			<Item Name="MNSN_HWCFG_HdwIO.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_HdwIO.vi"/>
			<Item Name="MNSN_HWCFG_Method.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_Method.vi"/>
			<Item Name="MNSN_HWCFG_CheckStatus.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_CheckStatus.vi"/>
			<Item Name="MNSN_actions.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_actions.ctl"/>
			<Item Name="MNSN_ReadATT.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadATT.vi"/>
			<Item Name="DHE_DRV_get_attribute.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_attribute.vi"/>
			<Item Name="MNSN_HWCF_MethodSIMPLE.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCF_MethodSIMPLE.vi"/>
			<Item Name="MNSN_HWCFG_Conversion.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_Conversion.vi"/>
			<Item Name="MNSN_HWCFG_POWERConversion.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_POWERConversion.vi"/>
			<Item Name="MNSN_HWCFG_TABLE1Conversion.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_TABLE1Conversion.vi"/>
			<Item Name="MNSN_HWCFG_LINEARConversion.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_LINEARConversion.vi"/>
			<Item Name="MNSN_WriteATT.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_WriteATT.vi"/>
			<Item Name="DHE_DRV_set_attribute.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_attribute.vi"/>
			<Item Name="MNSN_HWCF_MethodRDMSKWRT.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCF_MethodRDMSKWRT.vi"/>
			<Item Name="MNSN_HWCFG_ReplaceRegsVals.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_ReplaceRegsVals.vi"/>
			<Item Name="MNSN_sendAsync.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_sendAsync.vi"/>
			<Item Name="MNSN_HWCFG_GetParams.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_GetParams.vi"/>
			<Item Name="MNSN_HWCFG_GetList.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_GetList.vi"/>
			<Item Name="MNSN_slMon.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_slMon.vi"/>
			<Item Name="DHE_DRV_reset_pci.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_reset_pci.vi"/>
			<Item Name="DHE_DRV_set_monpath.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_monpath.vi"/>
			<Item Name="DHE_DRV_open_driver.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_open_driver.vi"/>
			<Item Name="MNSN_SetCmdAdd.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SetCmdAdd.vi"/>
			<Item Name="DHE_DRV_clean_addresses.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_clean_addresses.vi"/>
			<Item Name="DHE_DRV_add_cmdaddr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_add_cmdaddr.vi"/>
			<Item Name="MNSN_cmdaddStruct.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_cmdaddStruct.ctl"/>
			<Item Name="MNSN_PagesMngr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_PagesMngr.vi"/>
			<Item Name="MNSN_GetCatFromFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetCatFromFile.vi"/>
			<Item Name="MNSN_PageStruct.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_PageStruct.ctl"/>
			<Item Name="DHE_DRV_reset_controller.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_reset_controller.vi"/>
			<Item Name="MNSN_cmdAddMngr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_cmdAddMngr.vi"/>
			<Item Name="MNSN_Memory.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Memory.vi"/>
			<Item Name="MNSN_Memory_Range.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Memory_Range.vi"/>
			<Item Name="MNSN_ComposeMemAddress.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ComposeMemAddress.vi"/>
			<Item Name="DHE_DRV_read_memory.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_read_memory.vi"/>
			<Item Name="DHE_DRV_write_memory.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_write_memory.vi"/>
			<Item Name="DHE_DRV_load_file.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_load_file.vi"/>
			<Item Name="DHE_DRV_get_strresp.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_strresp.vi"/>
			<Item Name="MNSN_InitGeometry.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_InitGeometry.vi"/>
			<Item Name="DHE_DRV_dimensions.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_dimensions.vi"/>
			<Item Name="MNSN_ReadModes.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadModes.vi"/>
			<Item Name="DHE_DRV_set_bpp.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_bpp.vi"/>
			<Item Name="DHE_DRV_set_geom.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_geom.vi"/>
			<Item Name="DHE_DRV_set_roi.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_roi.vi"/>
			<Item Name="MNSN_ExtractInfo.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ExtractInfo.vi"/>
			<Item Name="DHE_DRV_get_controller_info.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_controller_info.vi"/>
			<Item Name="DHE_DRV_get_info.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_info.vi"/>
			<Item Name="DHE_DescribeDriverErrors.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_DescribeDriverErrors.vi"/>
			<Item Name="CMN_GetStrErrors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetStrErrors.vi"/>
			<Item Name="MNSN.vi" Type="VI" URL="../Modules/MNSN/public/vis/MNSN.vi"/>
			<Item Name="DHE_DRV_abort_all.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_abort_all.vi"/>
			<Item Name="DHE_DRV_artif_data.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_artif_data.vi"/>
			<Item Name="DHE_DRV_autoclear.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_autoclear.vi"/>
			<Item Name="DHE_DRV_autoshutter.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_autoshutter.vi"/>
			<Item Name="DHE_DRV_stop_idle.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_stop_idle.vi"/>
			<Item Name="DHE_DRV_resume_idle.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_resume_idle.vi"/>
			<Item Name="DHE_DRV_binning.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_binning.vi"/>
			<Item Name="MNSN_sendCmdAsyncs.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_sendCmdAsyncs.vi"/>
			<Item Name="DHE_DRV_clear_array.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_clear_array.vi"/>
			<Item Name="DHE_DRV_clear_progress_buf.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_clear_progress_buf.vi"/>
			<Item Name="DHE_DRV_close_driver.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_close_driver.vi"/>
			<Item Name="DHE_DRV_get_link_info.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_link_info.vi"/>
			<Item Name="MNSN_GetFrameTimeFromHardware.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetFrameTimeFromHardware.vi"/>
			<Item Name="DHE_DRV_read_frame_time.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_read_frame_time.vi"/>
			<Item Name="DHE_DRV_set_soft_read_time.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_soft_read_time.vi"/>
			<Item Name="DHE_GetPixReadTimeAlgors.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetPixReadTimeAlgors.vi"/>
			<Item Name="DHE_DRV_read_pix_time.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_read_pix_time.vi"/>
			<Item Name="DHE_GetPixWriteTimeAlgors.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetPixWriteTimeAlgors.vi"/>
			<Item Name="DHE_DRV_set_pixtime.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_pixtime.vi"/>
			<Item Name="DHE_DRV_move_shutter.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_move_shutter.vi"/>
			<Item Name="DHE_DRV_pause_exposure.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_pause_exposure.vi"/>
			<Item Name="DHE_DRV_power.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_power.vi"/>
			<Item Name="DHE_DRV_resume_exposure.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_resume_exposure.vi"/>
			<Item Name="DHE_DRV_set_expCode.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_expCode.vi"/>
			<Item Name="DHE_DRV_set_exp_time.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_exp_time.vi"/>
			<Item Name="DHE_DRV_set_nimages.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_nimages.vi"/>
			<Item Name="MNSN_SetSampling.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SetSampling.vi"/>
			<Item Name="DHE_DRV_set_num_reads.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_num_reads.vi"/>
			<Item Name="DHE_DRV_set_num_sur.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_num_sur.vi"/>
			<Item Name="DHE_DRV_set_num_coadds.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_num_coadds.vi"/>
			<Item Name="DHE_DRV_set_cont_read.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_cont_read.vi"/>
			<Item Name="DHE_GetPrePostNumFrames.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetPrePostNumFrames.vi"/>
			<Item Name="MNSN_HWCFG.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG.vi"/>
			<Item Name="MNSN_SelectGroup.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SelectGroup.vi"/>
			<Item Name="MNSN_ATT_GetGroup.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ATT_GetGroup.vi"/>
			<Item Name="MNSN_HWCFG_parse_request.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_parse_request.vi"/>
			<Item Name="DHE_DRV_stop_exposure.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_stop_exposure.vi"/>
			<Item Name="DHE_DRV_stop_sequence.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_stop_sequence.vi"/>
			<Item Name="MNSN_Page2AttList.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Page2AttList.vi"/>
			<Item Name="MNSN_REGS.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_REGS.vi"/>
			<Item Name="MNSN_Registers.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Registers.vi"/>
			<Item Name="MNSN_Fields2Values.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Fields2Values.vi"/>
			<Item Name="CMN_number2binarystr.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_number2binarystr.vi"/>
			<Item Name="MNSN_Register.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Register.ctl"/>
			<Item Name="MNSN_RegField.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_RegField.ctl"/>
			<Item Name="MNSN_Reg_Value2Fields.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_Reg_Value2Fields.vi"/>
			<Item Name="MNSN_REgister2Str.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_REgister2Str.vi"/>
			<Item Name="MNSN_WriteRegister.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_WriteRegister.vi"/>
			<Item Name="DHE_DO_IMREAD.vi" Type="VI" URL="../Modules/MNSN/private/vis/DHE_DO_IMREAD.vi"/>
			<Item Name="DHE_DO_IMREAD_NORMAL.vi" Type="VI" URL="../Modules/MNSN/private/vis/DHE_DO_IMREAD_NORMAL.vi"/>
			<Item Name="DHE_ShareNewObs.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_ShareNewObs.vi"/>
			<Item Name="DHE_GetobsID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetobsID.vi"/>
			<Item Name="MNSN_PrepareExposure.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_PrepareExposure.vi"/>
			<Item Name="DHE_SetEndReadFlag.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetEndReadFlag.vi"/>
			<Item Name="DHE_DRV_get_cont_read.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_cont_read.vi"/>
			<Item Name="CMN_toLogical.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toLogical.vi"/>
			<Item Name="DHE_DRV_feedBuffers.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_feedBuffers.vi"/>
			<Item Name="DHE_GetEndReadFlag.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetEndReadFlag.vi"/>
			<Item Name="MNSN_ExposureLoop.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ExposureLoop.vi"/>
			<Item Name="DHE_GetBufError.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetBufError.vi"/>
			<Item Name="DHE_DRV_expose.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_expose.vi"/>
			<Item Name="MNSN_ReadNFrames.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadNFrames.vi"/>
			<Item Name="DHE_DRV_readNframes.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_readNframes.vi"/>
			<Item Name="DHE_DRV_get_expose_status.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_expose_status.vi"/>
			<Item Name="MNSN_ExposureStatusLoop.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ExposureStatusLoop.vi"/>
			<Item Name="DHE_DRV_get_imstatus.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_imstatus.vi"/>
			<Item Name="CMN_FVAR_images_discard.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_images_discard.vi"/>
			<Item Name="MNSN_GetUTSHUT.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetUTSHUT.vi"/>
			<Item Name="DHE_ShareNewExp.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_ShareNewExp.vi"/>
			<Item Name="DHE_GetexposureID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetexposureID.vi"/>
			<Item Name="DHE_DefaultLogicProc.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_DefaultLogicProc.vi"/>
			<Item Name="DHE_chkFrameErr.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_chkFrameErr.vi"/>
			<Item Name="DHE_chkStopSequence.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_chkStopSequence.vi"/>
			<Item Name="DHE_setup_image.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_setup_image.vi"/>
			<Item Name="DHE_GetNumberFromName.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetNumberFromName.vi"/>
			<Item Name="MNSN_ExpCleanup.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ExpCleanup.vi"/>
			<Item Name="GEOM_NOAOCalculateHeadersME.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_NOAOCalculateHeadersME.vi"/>
			<Item Name="GEOM_NOAOCalculateHeadersS.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_NOAOCalculateHeadersS.vi"/>
			<Item Name="Proc_DCS.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_DCS.vi"/>
			<Item Name="GetAndLockBuffers.vi" Type="VI" URL="../Modules/buffermgr/GetAndLockBuffers.vi"/>
			<Item Name="LOGIC_GetNextImage.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_GetNextImage.vi"/>
			<Item Name="CMN_FVAR_coadds_done.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FVAR_coadds_done.vi"/>
			<Item Name="LOGIC_WaitNextFrame.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_WaitNextFrame.vi"/>
			<Item Name="LOGIC_chkStopSequence.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_chkStopSequence.vi"/>
			<Item Name="LOGIC_chkAndClearDiscardedImage.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_chkAndClearDiscardedImage.vi"/>
			<Item Name="deleteFrames.vi" Type="VI" URL="../Modules/buffermgr/framesMgr.llb/deleteFrames.vi"/>
			<Item Name="LOGIC_dcs.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_dcs.vi"/>
			<Item Name="CMN_inspectErros.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_inspectErros.vi"/>
			<Item Name="CMN_StrErrors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_StrErrors.vi"/>
			<Item Name="LOGIC_writeSubstracted.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_writeSubstracted.vi"/>
			<Item Name="LOGIC_WriteDCSFrames.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_WriteDCSFrames.vi"/>
			<Item Name="LOGIC_gbls.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_gbls.vi"/>
			<Item Name="UnlockBuffers.vi" Type="VI" URL="../Modules/buffermgr/UnlockBuffers.vi"/>
			<Item Name="Proc_FOWLER.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_FOWLER.vi"/>
			<Item Name="LOGIC_fowler.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_fowler.vi"/>
			<Item Name="LOGIC_chkAndClearCoaddDiscarded.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_chkAndClearCoaddDiscarded.vi"/>
			<Item Name="LOGIC_chkStopCoadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_chkStopCoadd.vi"/>
			<Item Name="LOGIC_coadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_coadd.vi"/>
			<Item Name="libLogic.so" Type="Document" URL="../Modules/Logical/public/c/lib/libLogic.so"/>
			<Item Name="LOGIC_writeCoadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_writeCoadd.vi"/>
			<Item Name="LOGIC_normalize.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_normalize.vi"/>
			<Item Name="LOGIC_writeNormalized.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_writeNormalized.vi"/>
			<Item Name="LOGIC_SendError.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_SendError.vi"/>
			<Item Name="Proc_SUR.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_SUR.vi"/>
			<Item Name="LOGIC_SUR.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_SUR.vi"/>
			<Item Name="LOGIC_fowProcOneSet_2.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_fowProcOneSet_2.vi"/>
			<Item Name="LOGIC_Unsc_and_Write_Buffer.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_Unsc_and_Write_Buffer.vi"/>
			<Item Name="Proc_ccd.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_ccd.vi"/>
			<Item Name="CMN_GetMaxElePerBuffer.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetMaxElePerBuffer.vi"/>
			<Item Name="LOGIC_set_imname.vi" Type="VI" URL="../Modules/Logical/Algors/LOGIC_set_imname.vi"/>
			<Item Name="LOGIC_ccd_setup_image.vi" Type="VI" URL="../Modules/Logical/Algors/LOGIC_ccd_setup_image.vi"/>
			<Item Name="Proc_ccdIV.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_ccdIV.vi"/>
			<Item Name="LOGIC_ChkFrameErr.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_ChkFrameErr.vi"/>
			<Item Name="fawProcOneSet.vi" Type="VI" URL="../Modules/Logical/Algors/fawProcOneSet.vi"/>
			<Item Name="fawProcOneSet_Line2Line.vi" Type="VI" URL="../Modules/Logical/Algors/fawProcOneSet_Line2Line.vi"/>
			<Item Name="fawProcOneSet_Pixel2Pixel.vi" Type="VI" URL="../Modules/Logical/Algors/fawProcOneSet_Pixel2Pixel.vi"/>
			<Item Name="GFITS_GetHdrTemplate.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetHdrTemplate.vi"/>
			<Item Name="GFITS_toFits.vi" Type="VI" URL="../Modules/GFITS/public/vis/GFITS_toFits.vi"/>
			<Item Name="GFITS_imageDataStruct.ctl" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_imageDataStruct.ctl"/>
			<Item Name="GFITS_GetImageParams.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetImageParams.vi"/>
			<Item Name="GFITS_GetImname.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetImname.vi"/>
			<Item Name="GFITS_writeFromBuffer.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_writeFromBuffer.vi"/>
			<Item Name="GFITS_LibWriteFromMemeFits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibWriteFromMemeFits.vi"/>
			<Item Name="GFITS_LibGetError.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibGetError.vi"/>
			<Item Name="GFITS_LOCKBUF.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LOCKBUF.vi"/>
			<Item Name="GFITS_SelectHdrFile.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SelectHdrFile.vi"/>
			<Item Name="GFITS_Readfile2fits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Readfile2fits.vi"/>
			<Item Name="GFITS_GetHeadTiming.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetHeadTiming.vi"/>
			<Item Name="GFITS_FindKeywordFromArray.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_FindKeywordFromArray.vi"/>
			<Item Name="GFITS_Fits2File.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Fits2File.vi"/>
			<Item Name="GFITS_KeyPreview.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_KeyPreview.vi"/>
			<Item Name="GFITS_PreviewHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_PreviewHeaders.vi"/>
			<Item Name="GFITS_getKeyvalueFromDBSHandler2.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_getKeyvalueFromDBSHandler2.vi"/>
			<Item Name="GFITS_Parse_comment.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Parse_comment.vi"/>
			<Item Name="GFITS_Parse_2DARR.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Parse_2DARR.vi"/>
			<Item Name="GFITS_GetDataTypes.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetDataTypes.vi"/>
			<Item Name="GFITS_HdrType2.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_HdrType2.vi"/>
			<Item Name="GFITS_ParseDataType.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ParseDataType.vi"/>
			<Item Name="GFITS_datatypesctl.ctl" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_datatypesctl.ctl"/>
			<Item Name="GFITS_file2fits2.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_file2fits2.vi"/>
			<Item Name="GFITS_GetGeometryHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetGeometryHeaders.vi"/>
			<Item Name="CMN_CallGeometry.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_CallGeometry.vi"/>
			<Item Name="GFITS_file2fitsMult.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_file2fitsMult.vi"/>
			<Item Name="GFITS_AnalizeFile.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_AnalizeFile.vi"/>
			<Item Name="GFITS_processOneExt.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_processOneExt.vi"/>
			<Item Name="GFITS_processOneExt_single.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_processOneExt_single.vi"/>
			<Item Name="GFITS_GetGeometryHeadersM.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetGeometryHeadersM.vi"/>
			<Item Name="GFITS_CollectHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_CollectHeaders.vi"/>
			<Item Name="GFITS_GetHdrType.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetHdrType.vi"/>
			<Item Name="GFITS_getKeyvalueFromDatabase.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_getKeyvalueFromDatabase.vi"/>
			<Item Name="GFITS_getInfo.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_getInfo.vi"/>
			<Item Name="GFITS_GetKeyValue.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetKeyValue.vi"/>
			<Item Name="CMN_toDBS.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toDBS.vi"/>
			<Item Name="GFITS_GetLocalValues.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetLocalValues.vi"/>
			<Item Name="GFITS_Gbls.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Gbls.vi"/>
			<Item Name="GFITS_getKeyvalueFromRemoteDBS.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_getKeyvalueFromRemoteDBS.vi"/>
			<Item Name="CMN_Str2_2DArr.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Str2_2DArr.vi"/>
			<Item Name="GFITS_ReadValueFromCtrl.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ReadValueFromCtrl.vi"/>
			<Item Name="GFITS_SetHdrTemplate.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetHdrTemplate.vi"/>
			<Item Name="GFITS_AddKeyword.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_AddKeyword.vi"/>
			<Item Name="CMN_SendAsyncMsg_dhe.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SendAsyncMsg_dhe.vi"/>
			<Item Name="GFITS_IMLOG.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_IMLOG.vi"/>
			<Item Name="GFITS_ImlistMngr.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ImlistMngr.vi"/>
			<Item Name="GFITS_WriteImNameToFile.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteImNameToFile.vi"/>
			<Item Name="GFITS_RemoveImNameFmList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_RemoveImNameFmList.vi"/>
			<Item Name="GFITS_FindImageFmList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_FindImageFmList.vi"/>
			<Item Name="GFITS_GetImList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetImList.vi"/>
			<Item Name="GFITS_ArrList2StrList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ArrList2StrList.vi"/>
			<Item Name="GFITS_GetDatesFmList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetDatesFmList.vi"/>
			<Item Name="GFITS_GetStatFmList.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetStatFmList.vi"/>
			<Item Name="GFITS_dhs_server.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_dhs_server.vi"/>
			<Item Name="GFITS_toInternalDHSFits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_toInternalDHSFits.vi"/>
			<Item Name="GFITS_NetServer.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_NetServer.vi"/>
			<Item Name="DHS_TCPSERVER_MsgStruct.ctl" Type="VI" URL="../Modules/GFITS/private/vis/DHS_TCPSERVER_MsgStruct.ctl"/>
			<Item Name="GFITS_NetServer2.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_NetServer2.vi"/>
			<Item Name="DRV_DHS_CmdStruct.ctl" Type="VI" URL="../Modules/DRV_DHS/private/vis/DRV_DHS_CmdStruct.ctl"/>
			<Item Name="GFITS_Dhs_CmdHandling.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Dhs_CmdHandling.vi"/>
			<Item Name="GFITS_Write2DHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Write2DHdrs.vi"/>
			<Item Name="GFITS_LibWriteHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibWriteHdrs.vi"/>
			<Item Name="GFITS_LibMoveHdu.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibMoveHdu.vi"/>
			<Item Name="GFITS_Write3DHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Write3DHdrs.vi"/>
			<Item Name="GFITS_SetHdrVar.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetHdrVar.vi"/>
			<Item Name="GFITS_GetQueued.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetQueued.vi"/>
			<Item Name="GFITS_LibImgageClose.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibImgageClose.vi"/>
			<Item Name="GFITS_CheckDir.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_CheckDir.vi"/>
			<Item Name="GFITS_ReplicateRemoteDir.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ReplicateRemoteDir.vi"/>
			<Item Name="GFITS_inspectErros.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_inspectErros.vi"/>
			<Item Name="GFITS_responseToQ.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_responseToQ.vi"/>
			<Item Name="GFITS_LibWrite.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibWrite.vi"/>
			<Item Name="GFITS_SetWriteProgress.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetWriteProgress.vi"/>
			<Item Name="GFITS_SeZcntProgress.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SeZcntProgress.vi"/>
			<Item Name="GFITS_ImageFinished.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ImageFinished.vi"/>
			<Item Name="CMN_GetUnscError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetUnscError.vi"/>
			<Item Name="GFITS_updateHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_updateHeaders.vi"/>
			<Item Name="GFITS_WriteHeaders_1ext.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteHeaders_1ext.vi"/>
			<Item Name="GFITS_LibHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibHeaders.vi"/>
			<Item Name="GFITS_WriteHeaders_quick.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteHeaders_quick.vi"/>
			<Item Name="GFITS_WriteHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteHeaders.vi"/>
			<Item Name="GFITS_PostNewFits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_PostNewFits.vi"/>
			<Item Name="GFITS_LibGetMemFits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibGetMemFits.vi"/>
			<Item Name="GFITS_LibGetMemBufPtr.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibGetMemBufPtr.vi"/>
			<Item Name="GFITS_LibWriteFromBuf.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibWriteFromBuf.vi"/>
			<Item Name="GFITS_imnameExtension.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_imnameExtension.vi"/>
			<Item Name="GFITS_LibsimpleDataWrite.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibsimpleDataWrite.vi"/>
			<Item Name="GFITS_LibInit.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibInit.vi"/>
			<Item Name="GFITS_LibClose.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibClose.vi"/>
			<Item Name="GFITS_SetImname.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetImname.vi"/>
			<Item Name="GFITS_LibSetMemFits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibSetMemFits.vi"/>
			<Item Name="DatabaseHand_FlushQvar.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_FlushQvar.vi"/>
			<Item Name="GFITS_GetEarlyHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetEarlyHeaders.vi"/>
			<Item Name="GFITS_DBSCollectHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_DBSCollectHeaders.vi"/>
			<Item Name="GFITS_Str2EXtensions.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Str2EXtensions.vi"/>
			<Item Name="GFITS_LibSetExtensions.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibSetExtensions.vi"/>
			<Item Name="GFITS_LibSetNzim.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibSetNzim.vi"/>
			<Item Name="GFITS_CloseIfValid.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_CloseIfValid.vi"/>
			<Item Name="GFITS_CreateImage.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_CreateImage.vi"/>
			<Item Name="GFITS_TryCreateImage.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_TryCreateImage.vi"/>
			<Item Name="GFITS_LibCreateImage.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibCreateImage.vi"/>
			<Item Name="GFITS_SeparateHeaders.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SeparateHeaders.vi"/>
			<Item Name="GFITS_API.vi" Type="VI" URL="../Modules/GFITS/public/vis/GFITS_API.vi"/>
			<Item Name="libgfits.so" Type="Document" URL="../Modules/GFITS/private/c/lib/libgfits.so"/>
			<Item Name="PAN.vi" Type="VI" URL="../public/vis/PAN.vi"/>
			<Item Name="GEOM_DetStruct.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_DetStruct.ctl"/>
			<Item Name="GEOM_evaluate_info_expression.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_evaluate_info_expression.vi"/>
			<Item Name="CMN_GetFromCmdLine.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetFromCmdLine.vi"/>
			<Item Name="CMN_GetFromEnv.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetFromEnv.vi"/>
			<Item Name="DatabaseHand_Dequeue.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Dequeue.vi"/>
			<Item Name="DatabaseHand_Element_Get(3DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(3DArr).vi"/>
			<Item Name="DatabaseHand_Element_Get(U64).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Get(U64).vi"/>
			<Item Name="GEOM_DetSummaryStruct.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_DetSummaryStruct.ctl"/>
			<Item Name="GEOM_VAR_OverrideWCS.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_OverrideWCS.vi"/>
			<Item Name="GEOM_GetWCSFile.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetWCSFile.vi"/>
			<Item Name="GEOM_CheckExtension.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CheckExtension.vi"/>
			<Item Name="GEOM_VAR_DataBasePath.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_DataBasePath.vi"/>
			<Item Name="GEOM_GetWCSOverride_data.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetWCSOverride_data.vi"/>
			<Item Name="GEOM_ParseOverridePars.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_ParseOverridePars.vi"/>
			<Item Name="GEOM_GetValsFromSection.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetValsFromSection.vi"/>
			<Item Name="GEOM_ParsePars.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_ParsePars.vi"/>
			<Item Name="GEOM_GetWCSOverride_data_mext.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_GetWCSOverride_data_mext.vi"/>
			<Item Name="PAN_VAR_Status.vi" Type="VI" URL="../public/vis/PAN_VAR_Status.vi"/>
			<Item Name="CMN_GetCommFile.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetCommFile.vi"/>
			<Item Name="DatabaseHand_Element_Set(3DArr).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(3DArr).vi"/>
			<Item Name="DatabaseHand_Element_Set(U64).vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_Element_Set(U64).vi"/>
			<Item Name="CMN_Parse Arithmetic Expression.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Parse Arithmetic Expression.vi"/>
			<Item Name="PAN_SeparateCmds.vi" Type="VI" URL="../private/vis/PAN_SeparateCmds.vi"/>
			<Item Name="PAN_ProcCmd.vi" Type="VI" URL="../private/vis/PAN_ProcCmd.vi"/>
			<Item Name="GEOM_ReadOverrideWCSFile.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadOverrideWCSFile.vi"/>
			<Item Name="GEOM_Line2Overrwcs2.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Line2Overrwcs2.vi"/>
			<Item Name="GEOM_GetIniConstants.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetIniConstants.vi"/>
			<Item Name="GEOM_VAR_Constants.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_Constants.vi"/>
			<Item Name="GEOM_ReplaceConstants.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReplaceConstants.vi"/>
			<Item Name="CMN_GetFromDBS.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetFromDBS.vi"/>
			<Item Name="GEOM_PickFormat.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_PickFormat.vi"/>
			<Item Name="GEOM_sustitute_size.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_sustitute_size.vi"/>
			<Item Name="GEOM_VAR_FPA2.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_FPA2.vi"/>
			<Item Name="GEOM_VAR_DETECTORS_uncorr.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_DETECTORS_uncorr.vi"/>
			<Item Name="GEOM_VAR_FPAReadModesUnBin.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_FPAReadModesUnBin.vi"/>
			<Item Name="GEOM_ReadCoordsReference.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ReadCoordsReference.vi"/>
			<Item Name="GEOM_VAR_CoordsRef.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_CoordsRef.vi"/>
			<Item Name="GEOM_CoordsRef.ctl" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CoordsRef.ctl"/>
			<Item Name="GEOM_MergeLevels.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_MergeLevels.vi"/>
			<Item Name="GEOM_GetSelectedAmps.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetSelectedAmps.vi"/>
			<Item Name="GEOM_findAmplifierbyCoordMult.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_findAmplifierbyCoordMult.vi"/>
			<Item Name="GEOM_Cleanup0amps.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Cleanup0amps.vi"/>
			<Item Name="GEOM_CorrectionAddition.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_CorrectionAddition.vi"/>
			<Item Name="GEOM_RestoreAmplifiersOrder.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_RestoreAmplifiersOrder.vi"/>
			<Item Name="GEOM_VAR_DETECTORS.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_DETECTORS.vi"/>
			<Item Name="GEOM_VAR_DETECTORS_uncorr_unbin.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_DETECTORS_uncorr_unbin.vi"/>
			<Item Name="GEOM_Check_optimize_time.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Check_optimize_time.vi"/>
			<Item Name="GEOM_AdjustDetStruct.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_AdjustDetStruct.vi"/>
			<Item Name="GEOM_Adjust1DetReadMode.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Adjust1DetReadMode.vi"/>
			<Item Name="GEOM_VAR_DETECTORS_corr_unbin.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_DETECTORS_corr_unbin.vi"/>
			<Item Name="GEOM_BinDetStruct.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_BinDetStruct.vi"/>
			<Item Name="GEOM_Bin1DetReadMode.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Bin1DetReadMode.vi"/>
			<Item Name="GEOM_ParseROIcmd.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_ParseROIcmd.vi"/>
			<Item Name="GEOM_GetRoi2Str.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetRoi2Str.vi"/>
			<Item Name="GEOM_SetExtensionsOrder.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetExtensionsOrder.vi"/>
			<Item Name="GEOM_DeleteOverrwcs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_DeleteOverrwcs.vi"/>
			<Item Name="GEOM_Line2Overrwcs.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_Line2Overrwcs.vi"/>
			<Item Name="GEOM_SetWCSMod.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_SetWCSMod.vi"/>
			<Item Name="GEOM_VAR_CustomKeys.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_VAR_CustomKeys.vi"/>
			<Item Name="GEOM_FormatGeomkeys.ctl" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_FormatGeomkeys.ctl"/>
			<Item Name="GEOM_HdrSec.ctl" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_HdrSec.ctl"/>
			<Item Name="GEOM_CoorSys.ctl" Type="VI" URL="../Modules/GEOM/private/vis/Headers/GEOM_CoorSys.ctl"/>
			<Item Name="GEOM_GetWCSDirectives.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetWCSDirectives.vi"/>
			<Item Name="GEOM_GetWCSMod.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetWCSMod.vi"/>
			<Item Name="PAN_DBS_parse_request.vi" Type="VI" URL="../private/vis/PAN_DBS_parse_request.vi"/>
			<Item Name="DatabaseHand_is_queue.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_is_queue.vi"/>
			<Item Name="DatabaseHand_Delete.vi" Type="VI" URL="../Modules/DBSHAND/public/vis/DatabaseHand_Delete.vi"/>
			<Item Name="DatabaseHand_DeleteSubElement.vi" Type="VI" URL="../Modules/DBSHAND/private/vis/DatabaseHand_DeleteSubElement.vi"/>
			<Item Name="PAN_DBS_is_queued.vi" Type="VI" URL="../private/vis/PAN_DBS_is_queued.vi"/>
			<Item Name="PAN_DBS_GetType.vi" Type="VI" URL="../private/vis/PAN_DBS_GetType.vi"/>
			<Item Name="DHE_SaveIDparams.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SaveIDparams.vi"/>
			<Item Name="DHE_GetSyncDir.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetSyncDir.vi"/>
			<Item Name="DHE_SaveImgparams.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SaveImgparams.vi"/>
			<Item Name="DHE_PrepareExp_cleanup.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_PrepareExp_cleanup.vi"/>
			<Item Name="DHE_Roi2Center.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_Roi2Center.vi"/>
			<Item Name="CMN_GetSpecificArg.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetSpecificArg.vi"/>
			<Item Name="DHE_SetNPixSamples.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SetNPixSamples.vi"/>
			<Item Name="DHE_set_seqdelay.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_set_seqdelay.vi"/>
			<Item Name="DHE_ParseBinning.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_ParseBinning.vi"/>
			<Item Name="DHE_arm.vi" Type="VI" URL="../Modules/DHE/private/driver/DHE_arm.vi"/>
			<Item Name="DHE_Modifiers2Str.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_Modifiers2Str.vi"/>
			<Item Name="DHE_GetDarkExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetDarkExpTime.vi"/>
			<Item Name="DHE_VAR_DarkExpTime.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_DarkExpTime.vi"/>
			<Item Name="DHE_TP_action.ctl" Type="VI" URL="../Modules/DHE/private/vis/DHE_TP_action.ctl"/>
			<Item Name="DHE_GetHardSemaphore.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetHardSemaphore.vi"/>
			<Item Name="DHE_ReleaseHardSemaphore.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_ReleaseHardSemaphore.vi"/>
			<Item Name="DHE_SaveSyncMacro.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_SaveSyncMacro.vi"/>
			<Item Name="DHE_GetSyncDir2.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_GetSyncDir2.vi"/>
			<Item Name="DHE_CheckExtension.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_CheckExtension.vi"/>
			<Item Name="DHE_SetShutdownMacro.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetShutdownMacro.vi"/>
			<Item Name="DHE_VAR_ShutdownMacro.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ShutdownMacro.vi"/>
			<Item Name="DHE_SetExtShutter.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetExtShutter.vi"/>
			<Item Name="DHE_VAR_ExtShutter.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_VAR_ExtShutter.vi"/>
			<Item Name="DHE_LoadImgparams.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_LoadImgparams.vi"/>
			<Item Name="DHE_LoadIIDs.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_LoadIIDs.vi"/>
			<Item Name="DHE_GetShutdownMacro.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetShutdownMacro.vi"/>
			<Item Name="DHE_FormatID.vi" Type="VI" URL="../Modules/DHE/private/vis/DHE_FormatID.vi"/>
			<Item Name="PAN_exe_system.vi" Type="VI" URL="../private/vis/PAN_exe_system.vi"/>
			<Item Name="PAN_ShutdownActions.vi" Type="VI" URL="../private/vis/PAN_ShutdownActions.vi"/>
			<Item Name="PAN_BuildDefModPath.vi" Type="VI" URL="../private/vis/PAN_BuildDefModPath.vi"/>
			<Item Name="PAN_AddModule.vi" Type="VI" URL="../private/vis/PAN_AddModule.vi"/>
			<Item Name="PAN_SendObsFinish.vi" Type="VI" URL="../public/vis/PAN_SendObsFinish.vi"/>
			<Item Name="PAN_ObsFinishStruct.ctl" Type="VI" URL="../private/vis/PAN_ObsFinishStruct.ctl"/>
			<Item Name="PAN_AnaResp.vi" Type="VI" URL="../private/vis/PAN_AnaResp.vi"/>
			<Item Name="PAN_GetCommModsInfo.vi" Type="VI" URL="../private/vis/PAN_GetCommModsInfo.vi"/>
			<Item Name="PAN_ReplaceMacroArgs.vi" Type="VI" URL="../private/vis/PAN_ReplaceMacroArgs.vi"/>
			<Item Name="PAN_ObsFinishMngr.vi" Type="VI" URL="../private/vis/PAN_ObsFinishMngr.vi"/>
			<Item Name="PAN_WaitObsDone.vi" Type="VI" URL="../public/vis/PAN_WaitObsDone.vi"/>
			<Item Name="PAN_ExpScript.vi" Type="VI" URL="../private/vis/PAN_ExpScript.vi"/>
			<Item Name="PAN_FinishCallback.vi" Type="VI" URL="../public/vis/PAN_FinishCallback.vi"/>
			<Item Name="MNSN_GetInitHardVals.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetInitHardVals.vi"/>
			<Item Name="DHE_DRV_set_extshutter.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_extshutter.vi"/>
			<Item Name="MNSN_OpenHardware.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_OpenHardware.vi"/>
			<Item Name="MNSN_BoardsHdwMngr.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_BoardsHdwMngr.vi"/>
			<Item Name="MNSN_MixHdwBoardTypes.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_MixHdwBoardTypes.vi"/>
			<Item Name="MNSN_BoardHdwStruct.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_BoardHdwStruct.ctl"/>
			<Item Name="MNSN_are_Cards_compatible.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_are_Cards_compatible.vi"/>
			<Item Name="MNSN_BoardSlotStruct.ctl" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_BoardSlotStruct.ctl"/>
			<Item Name="MNSN_RunScript.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_RunScript.vi"/>
			<Item Name="MNSN_GetMemDelay.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetMemDelay.vi"/>
			<Item Name="MNSN_GetSlotsFtomadd.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetSlotsFtomadd.vi"/>
			<Item Name="MNSN_GuessBoardsFromCsv.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GuessBoardsFromCsv.vi"/>
			<Item Name="MNSN_HWCFG_CompleteAliases.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_CompleteAliases.vi"/>
			<Item Name="CMN_Numb2Hex.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Numb2Hex.vi"/>
			<Item Name="MNSN_HWCFG_ReadSignalNamesFromFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_ReadSignalNamesFromFile.vi"/>
			<Item Name="MNSN_HWCFG_Read1LineFromSignalFile.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_HWCFG_Read1LineFromSignalFile.vi"/>
			<Item Name="CMN_Lab2CErrors.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_Lab2CErrors.vi"/>
			<Item Name="MNSN_ParseScanArgs.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ParseScanArgs.vi"/>
			<Item Name="MNSN_ScanBoards.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ScanBoards.vi"/>
			<Item Name="MNSN_MemDump.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_MemDump.vi"/>
			<Item Name="MNSN_SetMux.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SetMux.vi"/>
			<Item Name="MNSN_GetBoardFromAttName.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetBoardFromAttName.vi"/>
			<Item Name="MNSN_GetMuxClks.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetMuxClks.vi"/>
			<Item Name="MNSN_GetMux.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_GetMux.vi"/>
			<Item Name="MNSN_InitHardware.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_InitHardware.vi"/>
			<Item Name="DHE_GatherHdrInfo.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GatherHdrInfo.vi"/>
			<Item Name="MNSN_SetInitStatus.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SetInitStatus.vi"/>
			<Item Name="DHE_DRV_artif_pattern.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_artif_pattern.vi"/>
			<Item Name="DHE_DRV_arm.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_arm.vi"/>
			<Item Name="DHE_DRV_exec_seqvector.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_exec_seqvector.vi"/>
			<Item Name="DHE_DRV_clear_params.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_clear_params.vi"/>
			<Item Name="DHE_DRV_get_expdelay.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_expdelay.vi"/>
			<Item Name="DHE_GetExtShutter.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_GetExtShutter.vi"/>
			<Item Name="MNSN_SendCmd2Shutter.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SendCmd2Shutter.vi"/>
			<Item Name="MNSN_SelectTP.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_SelectTP.vi"/>
			<Item Name="DHE_DRV_set_expdelay.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_expdelay.vi"/>
			<Item Name="DHE_ReadExpTPs.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_ReadExpTPs.vi"/>
			<Item Name="MNSN_wait_for_shutter.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_wait_for_shutter.vi"/>
			<Item Name="DHE_DRV_get_shut_pos.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_shut_pos.vi"/>
			<Item Name="DHE_DRV_get_exptime.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_get_exptime.vi"/>
			<Item Name="DHE_DRV_set_shut_timer.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_set_shut_timer.vi"/>
			<Item Name="DHE_DRV_feedBuffers_light.vi" Type="VI" URL="../Modules/MNSN/private/vis/MNSN_ReadLib/DHE_DRV_feedBuffers_light.vi"/>
			<Item Name="DHE_SetDarkTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetDarkTime.vi"/>
			<Item Name="DHE_SetDarkExpTime.vi" Type="VI" URL="../Modules/DHE/public/vis/DHE_SetDarkExpTime.vi"/>
			<Item Name="GEOM_GetTofFPASize.vi" Type="VI" URL="../Modules/GEOM/private/vis/GEOM_GetTofFPASize.vi"/>
			<Item Name="GEOM_Convert2detheaders_NOAO.vi" Type="VI" URL="../Modules/GEOM/private/vis/Headers/Modules/GEOM_Convert2detheaders_NOAO.vi"/>
			<Item Name="Logic_clearMultMem.vi" Type="VI" URL="../Modules/Common/public/vis/Logic_clearMultMem.vi"/>
			<Item Name="LOGIC_LibOperateOnCoadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibOperateOnCoadd.vi"/>
			<Item Name="LOGIC_LibCoadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibCoadd.vi"/>
			<Item Name="LOGIC_LibOperateImages.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibOperateImages.vi"/>
			<Item Name="LOGIC_LibMemClear.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibMemClear.vi"/>
			<Item Name="LOGIC_CBlockBuffer2.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_CBlockBuffer2.vi"/>
			<Item Name="LOGIC_SendError2Fits.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_SendError2Fits.vi"/>
			<Item Name="LOGIC_Compute.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_Compute.vi"/>
			<Item Name="LOGIC_CreateWhen.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_CreateWhen.vi"/>
			<Item Name="Proc_multframes.vi" Type="VI" URL="../Modules/Logical/Algors/Proc_multframes.vi"/>
			<Item Name="LOGIC_multframes.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_multframes.vi"/>
			<Item Name="LOGIC_MsamplesProcOneSet.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_MsamplesProcOneSet.vi"/>
			<Item Name="LOGIC_LibResetSampCoaddCounters.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibResetSampCoaddCounters.vi"/>
			<Item Name="LOGIC_LibOperateOnPixSampCoadd.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_LibOperateOnPixSampCoadd.vi"/>
			<Item Name="LOGIC_WriteSample.vi" Type="VI" URL="../Modules/Logical/private/vis/LOGIC_WriteSample.vi"/>
			<Item Name="GFITS_exec_autocmds.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_exec_autocmds.vi"/>
			<Item Name="GFITS_SelectArrData.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SelectArrData.vi"/>
			<Item Name="GFITS_GetArrSubSet.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetArrSubSet.vi"/>
			<Item Name="GFITS_LibInitExtensions.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibInitExtensions.vi"/>
			<Item Name="GFITS_SetImNumFormat.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetImNumFormat.vi"/>
			<Item Name="GFITS_VAR_preallocHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_VAR_preallocHdrs.vi"/>
			<Item Name="GFITS_VAR_staticHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_VAR_staticHdrs.vi"/>
			<Item Name="GFITS_VAR_fstaticGeomHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_VAR_fstaticGeomHdrs.vi"/>
			<Item Name="GFITS_AsyncMsgs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_AsyncMsgs.vi"/>
			<Item Name="GFITS_PostProc.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_PostProc.vi"/>
			<Item Name="GFITS_ExecPostProc.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ExecPostProc.vi"/>
			<Item Name="libgfitspp.so" Type="Document" URL="../Modules/GFITS/private/c/lib/libgfitspp.so"/>
			<Item Name="GFITS_VAR_singleHdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_VAR_singleHdrs.vi"/>
			<Item Name="GFITS_VAR_Hdrs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_VAR_Hdrs.vi"/>
			<Item Name="GFITS_ReadAppendedHdrFiles.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ReadAppendedHdrFiles.vi"/>
			<Item Name="GFITS_header_operate.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_header_operate.vi"/>
			<Item Name="GFITS_CleanArr2fits.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_CleanArr2fits.vi"/>
			<Item Name="GFITS_GeomKeyPreview.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GeomKeyPreview.vi"/>
			<Item Name="GFITS_WriteFileMsg.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteFileMsg.vi"/>
			<Item Name="GFITS_toImstats.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_toImstats.vi"/>
			<Item Name="GFITS_WriteAmps.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_WriteAmps.vi"/>
			<Item Name="GFITS_Write.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_Write.vi"/>
			<Item Name="GFITS_FillHdrGbl.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_FillHdrGbl.vi"/>
			<Item Name="GFITS_LibSetNmemBufs.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibSetNmemBufs.vi"/>
			<Item Name="GFITS_SetCompression.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_SetCompression.vi"/>
			<Item Name="GFITS_LibSetShmem.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibSetShmem.vi"/>
			<Item Name="GFITS_LibGetShmem.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibGetShmem.vi"/>
			<Item Name="GFITS_LibGetExtensions.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_LibGetExtensions.vi"/>
			<Item Name="GFITS_GetExtmap.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetExtmap.vi"/>
			<Item Name="GFITS_ExecPreImg.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ExecPreImg.vi"/>
			<Item Name="GFITS_ImCreate.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_ImCreate.vi"/>
			<Item Name="GFITS_GetHeaderArray.vi" Type="VI" URL="../Modules/GFITS/private/vis/GFITS_GetHeaderArray.vi"/>
			<Item Name="GUNSC_LibGetMaxAmps.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibGetMaxAmps.vi"/>
			<Item Name="libunsc.so" Type="Document" URL="../Modules/GUNSC/public/c/lib/libunsc.so"/>
			<Item Name="GUNSC_LibOpen.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibOpen.vi"/>
			<Item Name="GUNSC_inspectErros.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_inspectErros.vi"/>
			<Item Name="GUNSC_LibShow.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibShow.vi"/>
			<Item Name="GUNSC_LibSetSwapPix.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetSwapPix.vi"/>
			<Item Name="GUNSC_LibSetArrangeSections.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSections.vi"/>
			<Item Name="GUNSC_LibSetArrangeSections_right.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSections_right.vi"/>
			<Item Name="GUNSC_parse_rearrange.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_parse_rearrange.vi"/>
			<Item Name="GUNSC_LibSetArrangeSections_left.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSections_left.vi"/>
			<Item Name="GUNSC_LibSetArrangeSectionsY.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSectionsY.vi"/>
			<Item Name="GUNSC_LibSetArrangeSections_upp.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSections_upp.vi"/>
			<Item Name="GUNSC_LibSetArrangeSections_low.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetArrangeSections_low.vi"/>
			<Item Name="GUNSC_LibEnable.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibEnable.vi"/>
			<Item Name="GUNSC_LibClearAmps.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibClearAmps.vi"/>
			<Item Name="GUNSC_LibGetFPA.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibGetFPA.vi"/>
			<Item Name="GUNSC_LibAddDet.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibAddDet.vi"/>
			<Item Name="GUNSC_LibDetExt.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibDetExt.vi"/>
			<Item Name="GUNSC_verifyNumExtensions.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_verifyNumExtensions.vi"/>
			<Item Name="GUNSC_LibAddAmp.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibAddAmp.vi"/>
			<Item Name="GUNSC_LibGetAmpsPtr.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibGetAmpsPtr.vi"/>
			<Item Name="CMN_toGUIDE.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toGUIDE.vi"/>
			<Item Name="CMN_toDSENDER.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_toDSENDER.vi"/>
			<Item Name="GUNSC_LibClose.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibClose.vi"/>
			<Item Name="GUNSC_Str2EXtensions.v.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_Str2EXtensions.v.vi"/>
			<Item Name="GUNSC_LibSetPhyMap.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetPhyMap.vi"/>
			<Item Name="GUNSC_GetPhysMap.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_GetPhysMap.vi"/>
			<Item Name="GUNSC_GetImageGeom.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_GetImageGeom.vi"/>
			<Item Name="GUNSC_LibSetFunction.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibSetFunction.vi"/>
			<Item Name="GUNSC_LibCopyImage.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibCopyImage.vi"/>
			<Item Name="GUNSC_LibGetRawPtr.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibGetRawPtr.vi"/>
			<Item Name="CMN_GetImprocError.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetImprocError.vi"/>
			<Item Name="GUNSC_LibUnscramble.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_LibUnscramble.vi"/>
			<Item Name="GUNSC_responseToQ.vi" Type="VI" URL="../Modules/GUNSC/private/vis/GUNSC_responseToQ.vi"/>
			<Item Name="TPNT_CleanAllData.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CleanAllData.vi"/>
			<Item Name="TPNT_DBSCore.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DBSCore.vi"/>
			<Item Name="TPNT_Not_a_Refnum.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Not_a_Refnum.vi"/>
			<Item Name="TPNT_Refnum.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Refnum.ctl"/>
			<Item Name="TPNT_CoreFunction.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CoreFunction.ctl"/>
			<Item Name="TPNT_DeviceTPs.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DeviceTPs.ctl"/>
			<Item Name="TPNT_TP.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_TP.ctl"/>
			<Item Name="TPNT_Limits.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Limits.ctl"/>
			<Item Name="TPNT_Samples.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Samples.ctl"/>
			<Item Name="TPNT_Log.ctl" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Log.ctl"/>
			<Item Name="TPNT_AlarmActions.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_AlarmActions.vi"/>
			<Item Name="TPNT_ParseAlarmField.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ParseAlarmField.vi"/>
			<Item Name="TPNT_DOAlarm.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DOAlarm.vi"/>
			<Item Name="TPNT_SendMail.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_SendMail.vi"/>
			<Item Name="TPND_DefaultMail.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPND_DefaultMail.vi"/>
			<Item Name="CMN_SendMail.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_SendMail.vi"/>
			<Item Name="TPNT_GetDDSStream.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDDSStream.vi"/>
			<Item Name="TPNT_ReadDataFromFile.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ReadDataFromFile.vi"/>
			<Item Name="TPNT_Args2TP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Args2TP.vi"/>
			<Item Name="TPNT_Args2DeviceInfo.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_Args2DeviceInfo.vi"/>
			<Item Name="TPNT_OpenDeviceData.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_OpenDeviceData.vi"/>
			<Item Name="TPNT_SetDeviceData.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_SetDeviceData.vi"/>
			<Item Name="TPNT_file2fits.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_file2fits.vi"/>
			<Item Name="CMN_FileTPL2Array.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_FileTPL2Array.vi"/>
			<Item Name="CMN_AnalizeFileTpl.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_AnalizeFileTpl.vi"/>
			<Item Name="CMN_processOneExt.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_processOneExt.vi"/>
			<Item Name="CMN_GetHdrType.vi" Type="VI" URL="../Modules/Common/public/vis/CMN_GetHdrType.vi"/>
			<Item Name="TPNT_SendCommand.vi" Type="VI" URL="../Modules/TPNT/public/vis/TPNT_SendCommand.vi"/>
			<Item Name="TPNT_PeriodicUpdate.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_PeriodicUpdate.vi"/>
			<Item Name="TPNT_CloseDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CloseDevice.vi"/>
			<Item Name="TPNT_AddDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_AddDevice.vi"/>
			<Item Name="TPNT_DeleteDeviceTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DeleteDeviceTP.vi"/>
			<Item Name="TPNT_FindDeviceTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_FindDeviceTP.vi"/>
			<Item Name="TPNT_GetDeviceData.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDeviceData.vi"/>
			<Item Name="TPNT_GetDeviceType.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDeviceType.vi"/>
			<Item Name="TPNT_ModifyDeviceTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ModifyDeviceTP.vi"/>
			<Item Name="TPNT_ModifyDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ModifyDevice.vi"/>
			<Item Name="TPNT_AddDeviceTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_AddDeviceTP.vi"/>
			<Item Name="TPNT_UpdateDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_UpdateDevice.vi"/>
			<Item Name="TPNT_GetTPfromDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetTPfromDevice.vi"/>
			<Item Name="TPNT_CallDevice.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CallDevice.vi"/>
			<Item Name="TPNT_ChkError.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ChkError.vi"/>
			<Item Name="TPNPT_Filter.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNPT_Filter.vi"/>
			<Item Name="TPNPT_AverageSamples.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNPT_AverageSamples.vi"/>
			<Item Name="TPNPT_MedianFilter.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNPT_MedianFilter.vi"/>
			<Item Name="TPNPT_MinorFilter.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNPT_MinorFilter.vi"/>
			<Item Name="TPNPT_MajorFilter.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNPT_MajorFilter.vi"/>
			<Item Name="TPNT_CheckInRange.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CheckInRange.vi"/>
			<Item Name="TPNT_CheckIfRegister.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CheckIfRegister.vi"/>
			<Item Name="TPNT_MarkTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_MarkTP.vi"/>
			<Item Name="TPNT_LogTP.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_LogTP.vi"/>
			<Item Name="TPNT_FormatLog.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_FormatLog.vi"/>
			<Item Name="TPNT_ReadLogFields.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_ReadLogFields.vi"/>
			<Item Name="TPNT_LogType.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_LogType.vi"/>
			<Item Name="TPNT_CheckFileSize.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CheckFileSize.vi"/>
			<Item Name="TPNT_WriteHTMLLog.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_WriteHTMLLog.vi"/>
			<Item Name="TPNT_WriteTextLog.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_WriteTextLog.vi"/>
			<Item Name="TPNT_UpdateHdrs.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_UpdateHdrs.vi"/>
			<Item Name="TPNT_SendAsyncStream.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_SendAsyncStream.vi"/>
			<Item Name="TPNT_GetValues.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetValues.vi"/>
			<Item Name="TPNT_StartAsyncServer.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_StartAsyncServer.vi"/>
			<Item Name="TPNT_StartAsyncDDSServer.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_StartAsyncDDSServer.vi"/>
			<Item Name="TPNT_GetTP2Str.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetTP2Str.vi"/>
			<Item Name="TPNT_TP2Str.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_TP2Str.vi"/>
			<Item Name="TPNT_CheckLimitConsistecy.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_CheckLimitConsistecy.vi"/>
			<Item Name="TPNT_GetDev2Str.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDev2Str.vi"/>
			<Item Name="TPNT_DevInfo2Str.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DevInfo2Str.vi"/>
			<Item Name="TPNT_DeviceInfo2Args.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_DeviceInfo2Args.vi"/>
			<Item Name="TPNT_GetTypes.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetTypes.vi"/>
			<Item Name="TPNT_GetAll2Str.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetAll2Str.vi"/>
			<Item Name="TPNT_GetByType.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetByType.vi"/>
			<Item Name="TPNT_GetDeviceHelp(all).vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDeviceHelp(all).vi"/>
			<Item Name="TPNT_GetDeviceHelp.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetDeviceHelp.vi"/>
			<Item Name="TPNT_GetHelp.vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetHelp.vi"/>
			<Item Name="TPNT_GetTPHelp(all).vi" Type="VI" URL="../Modules/TPNT/private/vis/TPNT_GetTPHelp(all).vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="panapp" Type="EXE">
				<Property Name="App_applicationGUID" Type="Str">{8986AED7-7DB2-11DC-AADC-0019D1E00053}</Property>
				<Property Name="App_applicationName" Type="Str">panapp</Property>
				<Property Name="App_enableDebugging" Type="Bool">true</Property>
				<Property Name="App_fileDescription" Type="Str">panapp</Property>
				<Property Name="App_fileVersion.major" Type="Int">1</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{8986ACB1-7DB2-11DC-AADC-0019D1E00053}</Property>
				<Property Name="App_INI_GUID" Type="Str">{8986AF8B-7DB2-11DC-AADC-0019D1E00053}</Property>
				<Property Name="App_internalName" Type="Str">panapp</Property>
				<Property Name="App_legalCopyright" Type="Str">Copyright 2007 </Property>
				<Property Name="App_productName" Type="Str">panapp</Property>
				<Property Name="Bld_buildSpecName" Type="Str">panapp</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Destination[0].destName" Type="Str">panapp</Property>
				<Property Name="Destination[0].path" Type="Path">../NI_AB_PROJECTNAME/public/bin/internal.llb</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../NI_AB_PROJECTNAME/public/bin/data</Property>
				<Property Name="SourceCount" Type="Int">14</Property>
				<Property Name="Source[0].itemID" Type="Str">{B9A8D71F-8EC6-11E0-AB96-0019D1E00053}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[10].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[10].itemID" Type="Ref">/My Computer/TPNT_Main.vi</Property>
				<Property Name="Source[10].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[10].type" Type="Str">VI</Property>
				<Property Name="Source[11].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[11].itemID" Type="Ref">/My Computer/TPNT_ExternalServer.vi</Property>
				<Property Name="Source[11].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[11].type" Type="Str">VI</Property>
				<Property Name="Source[12].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[12].itemID" Type="Ref">/My Computer/TPNT_TcpServer.vi</Property>
				<Property Name="Source[12].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[12].type" Type="Str">VI</Property>
				<Property Name="Source[13].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[13].itemID" Type="Ref">/My Computer/TPNT_API.vi</Property>
				<Property Name="Source[13].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[13].type" Type="Str">VI</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/GEOM_CalculateFPAINFOHeaders.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="Source[2].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[2].itemID" Type="Ref">/My Computer/GEOM_CalculateDETINFOHeaders.vi</Property>
				<Property Name="Source[2].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[2].type" Type="Str">VI</Property>
				<Property Name="Source[3].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[3].itemID" Type="Ref">/My Computer/GEOM_CalculateWCSHeaders.vi</Property>
				<Property Name="Source[3].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[3].type" Type="Str">VI</Property>
				<Property Name="Source[4].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[4].itemID" Type="Ref">/My Computer/PAN_run.vi</Property>
				<Property Name="Source[4].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[4].type" Type="Str">VI</Property>
				<Property Name="Source[5].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[5].itemID" Type="Ref">/My Computer/MNSN_Cmds.App.vi</Property>
				<Property Name="Source[5].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[5].type" Type="Str">VI</Property>
				<Property Name="Source[6].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[6].itemID" Type="Ref">/My Computer/GEOM_CalculateNOAOHeaders.vi</Property>
				<Property Name="Source[6].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[6].type" Type="Str">VI</Property>
				<Property Name="Source[7].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[7].itemID" Type="Ref">/My Computer/LOGIC_algors.vi</Property>
				<Property Name="Source[7].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[7].type" Type="Str">VI</Property>
				<Property Name="Source[8].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[8].itemID" Type="Ref">/My Computer/GFITS_Main.vi</Property>
				<Property Name="Source[8].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[8].type" Type="Str">VI</Property>
				<Property Name="Source[9].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[9].itemID" Type="Ref">/My Computer/GUNSC_Main.vi</Property>
				<Property Name="Source[9].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[9].type" Type="Str">VI</Property>
			</Item>
		</Item>
	</Item>
</Project>
