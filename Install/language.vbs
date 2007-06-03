If Session.Property("C3D_LANGUAGE") = "C3D_LANG_ESP" Then
	Session.FeatureRequestState("FEATURE_C3D_ESP") = 3
	Session.FeatureRequestState("FEATURE_C3D_ENG") = 2
	Session.FeatureRequestState("FEATURE_C3D_FRA") = 2
	Session.FeatureRequestState("FEATURE_C3D_PTG") = 2
ElseIf Session.Property("C3D_LANGUAGE") = "C3D_LANG_FRA" Then
	Session.FeatureRequestState("FEATURE_C3D_FRA") = 3
	Session.FeatureRequestState("FEATURE_C3D_ENG") = 2
	Session.FeatureRequestState("FEATURE_C3D_PTG") = 2
	Session.FeatureRequestState("FEATURE_C3D_ESP") = 2
ElseIf Session.Property("C3D_LANGUAGE") = "C3D_LANG_PTG" Then
	Session.FeatureRequestState("FEATURE_C3D_PTG") = 3
	Session.FeatureRequestState("FEATURE_C3D_ENG") = 2
	Session.FeatureRequestState("FEATURE_C3D_FRA") = 2
	Session.FeatureRequestState("FEATURE_C3D_ESP") = 2
Else
	Session.FeatureRequestState("FEATURE_C3D_ENG") = 3
	Session.FeatureRequestState("FEATURE_C3D_FRA") = 2
	Session.FeatureRequestState("FEATURE_C3D_PTG") = 2
	Session.FeatureRequestState("FEATURE_C3D_ESP") = 2
End If
