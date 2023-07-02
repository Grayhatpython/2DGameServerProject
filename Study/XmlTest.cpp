//XmlNode root;
//XmlParser parser;
//if (parser.ParseFromFile(L"GameDB.xml", OUT root) == false)
//return 0;
//
//Vector<XmlNode> tables = root.FindChildren(L"Table");
//for (XmlNode& table : tables)
//{
//	String name = table.GetStringAttr(L"name");
//	String desc = table.GetStringAttr(L"desc");
//
//	Vector<XmlNode> columns = table.FindChildren(L"Column");
//	for (XmlNode& column : columns)
//	{
//		String colName = column.GetStringAttr(L"name");
//		String colType = column.GetStringAttr(L"type");
//		bool nullable = !column.GetBoolAttr(L"notnull", false);
//		String identity = column.GetStringAttr(L"identity");
//		String colDefault = column.GetStringAttr(L"default");
//		// Etc...
//	}
//
//	Vector<XmlNode> indices = table.FindChildren(L"Index");
//	for (XmlNode& index : indices)
//	{
//		String indexType = index.GetStringAttr(L"type");
//		bool primaryKey = index.FindChild(L"PrimaryKey").IsValid();
//		bool uniqueConstraint = index.FindChild(L"UniqueKey").IsValid();
//
//		Vector<XmlNode> columns = index.FindChildren(L"Column");
//		for (XmlNode& column : columns)
//		{
//			String colName = column.GetStringAttr(L"name");
//		}
//	}
//}
//
//Vector<XmlNode> procedures = root.FindChildren(L"Procedure");
//for (XmlNode& procedure : procedures)
//{
//	String name = procedure.GetStringAttr(L"name");
//	String body = procedure.FindChild(L"Body").GetStringValue();
//
//	Vector<XmlNode> params = procedure.FindChildren(L"Param");
//	for (XmlNode& param : params)
//	{
//		String paramName = param.GetStringAttr(L"name");
//		String paramType = param.GetStringAttr(L"type");
//		// TODO..
//	}
//}