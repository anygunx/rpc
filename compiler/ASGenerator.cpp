#include "ASGenerator.h"
#include "Compiler.h"
#include "CodeFile.h"

static CodeFile* openFile(const std::string& name)
{
	std::string filename = 
		Compiler::inst().outputDir_ + 
		Compiler::inst().fileStem_ + 
		"/" + name + ".as";
	CodeFile* f = new CodeFile(filename);
	f->output("/* This file is generated by arpcc, do not change manually! */");
	// package.
	f->output("package %s", Compiler::inst().fileStem_.c_str());
	f->output("{");
	f->indent();
	// imports.
	f->output("import flash.utils.ByteArray;");
	f->output("import arpc.*;");
	for(std::set<std::string>::iterator iter = Compiler::inst().importedFiles_.begin();
		iter != Compiler::inst().importedFiles_.end(); ++iter)
	{
		std::string pkgName = *iter;
		pkgName = pkgName.substr(0, pkgName.find('.'));
		f->output("import %s.*;", pkgName.c_str());
	}
	return f;
}

static void closeFile(CodeFile* f)
{
	f->recover();
	f->output("}");
	delete f;
}

static void generateEnum(Enum* e)
{
	CodeFile* f = openFile(e->getName());
	f->output("public class %s", e->getNameC());
	f->output("{");
	f->indent();
	for(size_t i = 0; i < e->items_.size(); i++)
		f->output("public static const %s:uint = %d;", e->items_[i].c_str(), i);
	f->recover();
	f->output("}");
	closeFile(f);
}

static void generateSingleFieldSerializeCode(CodeFile* f, Field& field, const char* wn, bool skipComp)
{
	switch(field.getType())
	{
	case FT_INT64:
	case FT_UINT64:
		{
			if(!skipComp) { f->output("if(!%s.isZero())", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeInt64(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_DOUBLE:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeDouble(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_FLOAT:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeFloat(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_INT32:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeInt32(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_UINT32:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeUInt32(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_INT16:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeInt16(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_UINT16:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeUInt16(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_INT8:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeInt8(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_UINT8:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeUInt8(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_BOOL:
		{
			if(skipComp)
				f->output("ProtocolWriter.writeBool(%s, %s);", wn, field.getNameC());
		}
		break;
	case FT_ENUM:
		{
			if(!skipComp) { f->output("if(%s != 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeEnum(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_STRING:
		{
			if(!skipComp) { f->output("if(%s.length > 0)", field.getNameC()); f->indent(); }
			f->output("ProtocolWriter.writeString(%s, %s);", wn, field.getNameC());
			if(!skipComp) f->recover();
		}
		break;
	case FT_USER:
		{
			f->output("%s.serialize(%s);", field.getNameC(), wn);
		}
		break;
	}
}

static void generateArrayFieldSerializeCode(CodeFile* f, Field& field, const char* wn, bool skipComp)
{
	// Array size.
	if(!skipComp)
	{
		f->output("if(%s.length > 0)", field.getNameC(), field.getNameC());
		f->output("{");
		f->indent();
	}
	// write dynamic size.
	f->output("ProtocolWriter.writeDynSize(%s, %s.length);", wn, field.getNameC());
	if(field.getType() == FT_UINT8)
	{
		// bytes.
		f->output("ProtocolWriter.writeBytes(%s, %s);", wn, field.getNameC());
	}
	else
	{
		f->output("for(__i__ = 0; __i__ < %s.length; __i__++)", field.getNameC());
		f->output("{");
		f->indent();
		switch(field.getType())
		{
		case FT_INT64:
		case FT_UINT64:
			f->output("ProtocolWriter.writeInt64(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_DOUBLE:
			f->output("ProtocolWriter.writeDouble(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_FLOAT:
			f->output("ProtocolWriter.writeFloat(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_INT32:
			f->output("ProtocolWriter.writeInt32(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_UINT32:
			f->output("ProtocolWriter.writeUInt32(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_INT16:
			f->output("ProtocolWriter.writeInt16(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_UINT16:
			f->output("ProtocolWriter.writeUInt16(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_INT8:
			f->output("ProtocolWriter.writeInt8(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_BOOL:
			f->output("ProtocolWriter.writeBool(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_ENUM:
			f->output("ProtocolWriter.writeEnum(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_STRING:
			f->output("ProtocolWriter.writeString(%s, %s[__i__]);", wn, field.getNameC());
			break;
		case FT_USER:
			f->output("%s[__i__].serialize(%s);", field.getNameC(), wn);
			break;
		}
		f->recover();
		f->output("}");
	}
	if(!skipComp)
	{
		f->recover();
		f->output("}");
	}
}

static void generateFieldSerializeCode(CodeFile* f, Field& field, const char* wn, bool skipComp)
{
	if(field.getArray())
		generateArrayFieldSerializeCode(f, field, wn, skipComp);
	else
		generateSingleFieldSerializeCode(f, field, wn, skipComp);
}

static void generateFieldContainerSerializeCode(CodeFile* f, FieldContainer* fc, const char* wn, bool skipComp)
{
	if(!fc->fields_.size())
		return;
	if(!skipComp)
	{
		f->output("var __fm__:FieldMask = new FieldMask();");
		for(size_t i = 0; i < fc->fields_.size(); i++)
		{
			Field& field = fc->fields_[i];
			if(field.getArray())
				f->output("__fm__.writeBit(%s.length>0?true:false);", field.getNameC(), field.getNameC());
			else
			{
				switch(field.getType())
				{
				case FT_INT64:
				case FT_UINT64:
					f->output("__fm__.writeBit(!%s.isZero());", field.getNameC());
					break;
				case FT_DOUBLE:
				case FT_FLOAT:
				case FT_INT32:
				case FT_UINT32:
				case FT_INT16:
				case FT_UINT16:
				case FT_INT8:
				case FT_UINT8:
					f->output("__fm__.writeBit(%s==0?false:true);", field.getNameC());
					break;
				case FT_BOOL:
					f->output("__fm__.writeBit(%s);", field.getNameC());
					break;
				case FT_ENUM:
					f->output("__fm__.writeBit(%s==0?false:true);", field.getNameC());
					break;
				case FT_STRING:
					f->output("__fm__.writeBit(%s.length>0?true:false);", field.getNameC());
					break;
				case FT_USER:
					f->output("__fm__.writeBit(true);");
					break;
				}
			}
		}
		f->output("ProtocolWriter.writeBytes(%s, __fm__.getMasks());", wn);
	}
	for(size_t i = 0; i < fc->fields_.size(); i++)
		generateFieldSerializeCode(f, fc->fields_[i], wn, skipComp);
}

static void generateSingleFieldDeserializeCode(CodeFile* f, Field& field, const char* rn, bool skipComp)
{
	switch(field.getType())
	{
	case FT_INT64:
	case FT_UINT64:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readInt64(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_DOUBLE:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readDouble(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_FLOAT:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readFloat(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_INT32:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readInt32(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_UINT32:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readUInt32(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_INT16:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readInt16(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_UINT16:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readUInt16(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_INT8:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readInt8(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_UINT8:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readUInt8(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_BOOL:
		{
			if(!skipComp) 
				f->output("%s = __fm__.readBit();", field.getNameC());
			else
				f->output("%s = ProtocolReader.readBool(%s);", field.getNameC(), rn);
		}
		break;
	case FT_ENUM:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readEnum(%s, %d);", field.getNameC(), rn, field.getUserType()->getEnum()->items_.size());
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_STRING:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s = ProtocolReader.readString(%s, %d);", field.getNameC(), rn, field.getMaxStrLength());
			if(!skipComp) { f->recover(); }
		}
		break;
	case FT_USER:
		{
			if(!skipComp) { f->output("if(__fm__.readBit())"); f->indent(); }
			f->output("%s.deserialize(%s);", field.getNameC(), rn);
			if(!skipComp) { f->recover(); }
		}
		break;
	}
}

static void generateArrayFieldDeserializeCode(CodeFile* f, Field& field, const char* rn, bool skipComp)
{
	if(!skipComp)
	{
		f->output("if(__fm__.readBit())");
		f->output("{");
		f->indent();
	}
	f->output("__len__ = ProtocolReader.readDynSize(%s, %d);", rn, field.getMaxArrLength());
	if(field.getType() == FT_UINT8)
	{
		// bytes.
		f->output("ProtocolReader.readBytes(%s, %s, __len__);", rn, field.getNameC());
	}
	else
	{
		f->output("for(__i__ = 0; __i__ < __len__; __i__++)");
		f->output("{");
		f->indent();
		switch(field.getType())
		{
		case FT_INT64:
		case FT_UINT64:
			f->output("%s.push(ProtocolReader.readInt64(%s));", field.getNameC(), rn);
			break;
		case FT_DOUBLE:
			f->output("%s.push(ProtocolReader.readDouble(%s));", field.getNameC(), rn);
			break;
		case FT_FLOAT:
			f->output("%s.push(ProtocolReader.readFloat(%s));", field.getNameC(), rn);
			break;
		case FT_INT32:
			f->output("%s.push(ProtocolReader.readInt32(%s));", field.getNameC(), rn);
			break;
		case FT_UINT32:
			f->output("%s.push(ProtocolReader.readUInt32(%s));", field.getNameC(), rn);
			break;
		case FT_INT16:
			f->output("%s.push(ProtocolReader.readInt16(%s));", field.getNameC(), rn);
			break;
		case FT_UINT16:
			f->output("%s.push(ProtocolReader.readUInt16(%s));", field.getNameC(), rn);
			break;
		case FT_INT8:
			f->output("%s.push(ProtocolReader.readInt8(%s));", field.getNameC(), rn);
			break;
		case FT_BOOL:
			f->output("%s.push(ProtocolReader.readBool(%s));", field.getNameC(), rn);
			break;
		case FT_ENUM:
			f->output("%s.push(ProtocolReader.readEnum(%s, %d));", field.getNameC(), rn, field.getUserType()->getEnum()->items_.size());
			break;
		case FT_STRING:
			f->output("%s.push(ProtocolReader.readString(%s, %d));", field.getNameC(), rn, field.getMaxStrLength());
			break;
		case FT_USER:
			f->output("%s.push(new %s());", field.getNameC(), field.getUserType()->getNameC());
			f->output("%s[__i__].deserialize(%s);", field.getNameC(), rn);
			break;
		}
		f->recover();
		f->output("}");
	}
	if(!skipComp)
	{
		f->recover();
		f->output("}");
	}
}

static void generateFieldDeserializeCode(CodeFile* f, Field& field, const char* rn, bool skipComp)
{
	if(field.getArray())
		generateArrayFieldDeserializeCode(f, field, rn, skipComp);
	else
		generateSingleFieldDeserializeCode(f, field, rn, skipComp);
}

static void generateFieldContainerDeserializeCode(CodeFile* f, FieldContainer* fc, const char* rn, bool skipComp)
{
	if(!fc->fields_.size())
		return;
	if(!skipComp)
	{
		f->output("var __fm__:FieldMask = new FieldMask();");
		f->output("ProtocolReader.readBytes(%s, __fm__.getMasks(), %d);", rn, fc->getFMByteNum());
	}
	for(size_t i = 0; i < fc->fields_.size(); i++)
		generateFieldDeserializeCode(f, fc->fields_[i], rn, skipComp);
}

static const char* getFieldType(Field& field)
{
	static std::string t;
	t = "";
	if(field.getType() == FT_UINT8 && field.getArray())
		t = "ByteArray";
	else
	{
		if(field.getArray())	
			t = "Vector.<";
		switch(field.getType())
		{
		case FT_INT64:
		case FT_UINT64: t += "Int64"; break;
		case FT_DOUBLE:
		case FT_FLOAT:	t += "Number"; break;
		case FT_INT32:
		case FT_INT16:
		case FT_INT8:	t += "int"; break;
		case FT_ENUM:
		case FT_UINT32:
		case FT_UINT16:
		case FT_UINT8:	t += "uint"; break;
		case FT_BOOL:	t += "Boolean"; break;
		case FT_STRING:	t += "String"; break;
		case FT_USER:	t += field.getUserType()->getName(); break;
		}
		if(field.getArray())
			t += ">";
	}
	return t.c_str();
}

static void generateStruct(Struct* s)
{
	CodeFile* f = openFile(s->getName());
	if(s->super_)
		f->output("public class %s extends %s", s->getNameC(), s->super_->getNameC());
	else
		f->output("public class %s", s->getNameC());
	f->output("{");
	f->indent();
	// fields.
	f->output("// fields.");
	for(size_t i = 0; i < s->fields_.size(); i++)
	{
		Field& field = s->fields_[i];
		f->begin();
		f->append("public var %s:%s", field.getNameC(), getFieldType(field));
		if(field.getArray() || 
			field.getType() == FT_STRING || 
			field.getType() == FT_USER || 
			field.getType() == FT_INT64 ||
			field.getType() == FT_UINT64)
			f->append(" = new %s();", getFieldType(field));
		else
			f->append(";");
		f->end();
	}
	/** Field ids. */
	f->output("// field ids.");
	size_t fid = s->super_?s->super_->getFieldNum():0;
	for(size_t i = 0; i < s->fields_.size(); i++)
	{
		Field& field = s->fields_[i];
		f->output("public static const FID_%s:uint = %d;", field.getNameC(), fid);
		fid++;
	}
	f->output("public static const FID_MAX:uint = %d;", fid);

	// serialize code.
	f->output("// serialize.");
	f->output("public %sfunction serialize(w:IWriter):void", s->super_?"override ":"");
	f->output("{");
	f->indent();
	if(s->super_)
		f->output("super.serialize(w);");
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	generateFieldContainerSerializeCode(f, s, "w", s->skipComp_);
	f->recover();
	f->output("}");
	// deserialize code.
	f->output("// deserialize.");
	f->output("public %sfunction deserialize(r:IReader):void", s->super_?"override ":"");
	f->output("{");
	f->indent();
	if(s->super_)
		f->output("super.deserialize(r);");
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	generateFieldContainerDeserializeCode(f, s, "r", s->skipComp_);
	f->recover();
	f->output("}");
	// field serialize.
	f->output("// serializeField.");
	f->output("public %sfunction serializeField(fid:uint, w:IWriter):void", s->super_?"override ":"");
	f->output("{");
	f->indent();
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	f->output("switch(fid)");
	f->output("{");
	f->indent();
	for(size_t i = 0; i < s->fields_.size(); i++)
	{
		Field& field = s->fields_[i];
		f->output("case FID_%s:", field.getNameC());
		f->output("{");
		f->indent();
		generateFieldSerializeCode(f, field, "w", true);
		f->recover();
		f->output("}");
		f->output("return;");
	}
	f->recover();
	f->output("}");
	if(s->super_)
		f->output("super.serializeField(fid, w);");
	else
		f->output("throw \"invalid field id\";");
	f->recover();
	f->output("}");
	// field deserialize.
	f->output("// deserializeField.");
	f->output("public %sfunction deserializeField(fid:uint, r:IReader):void", s->super_?"override ":"");
	f->output("{");
	f->indent();
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	f->output("switch(fid)");
	f->output("{");
	f->indent();
	for(size_t i = 0; i < s->fields_.size(); i++)
	{
		Field& field = s->fields_[i];
		f->output("case FID_%s:", field.getNameC());
		f->output("{");
		f->indent();
		generateFieldDeserializeCode(f, field, "r", true);
		f->recover();
		f->output("}");
		f->output("return;");
	}
	f->recover();
	f->output("}");
	if(s->super_)
		f->output("super.deserializeField(fid, r);");
	else
		f->output("throw \"invalid field id\";");
	f->recover();
	f->output("}");

	f->recover();
	f->output("}");
	closeFile(f);
}

static void generateStubMethod(CodeFile* f, Service* s, Method& m, size_t mid)
{
	f->begin();
	f->append("public function %s(", m.getNameC());
	for(size_t i = 0; i < m.fields_.size(); i++)
	{
		Field& field = m.fields_[i];
		f->append("%s:%s%s", field.getNameC(), getFieldType(field), (i == m.fields_.size()-1)?"":",");
	}
	f->append("):void");
	f->end();
	f->output("{");
	f->indent();

	f->output("var w:IWriter = methodBegin();");
	f->output("if(w == null) return;");
	f->output("ProtocolWriter.writeUInt16(w, %d);", mid);
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	generateFieldContainerSerializeCode(f, &m, "w", true);
	f->output("methodEnd();");

	f->recover();
	f->output("}");
}

static void generateServiceStub(Service* s)
{
	CodeFile* f = openFile(s->getName() + "Stub");
	if(s->super_)
		f->output("public class %sStub extends %sStub", s->getNameC(), s->super_->getNameC());
	else
		f->output("public class %sStub", s->getNameC());
	f->output("{");
	f->indent();
	if(!s->super_)
	{
		f->output("protected function methodBegin():IWriter { return null; }");
		f->output("protected function methodEnd():void {}");
	}
	// methods.
	size_t methodStartId = s->super_?s->super_->getMethodNum():0;
	for(size_t i = 0; i < s->methods_.size(); i++)
		generateStubMethod(f, s, s->methods_[i], methodStartId + i);
	f->recover();
	f->output("}");
	closeFile(f);
}

static void generateProxyAbstractMethod(CodeFile* f, Method& m)
{
	f->begin();
	f->append("function %s(", m.getNameC());
	for(size_t i = 0; i < m.fields_.size(); i++)
	{
		Field& field = m.fields_[i];
		f->append("%s:%s%s", field.getNameC(), getFieldType(field), (i == m.fields_.size()-1)?"":",");
	}
	f->append("):void;");
	f->end();
}

static void generateMethodDispatcher(CodeFile* f, Service* s, Method& m)
{
	f->output("protected static function %s(__r__:IReader, __p__:%sProxy):void", m.getNameC(), s->getNameC());
	f->output("{");
	f->indent();
	// fields.
	for(size_t i = 0; i < m.fields_.size(); i++)
	{
		Field& field = m.fields_[i];
		f->begin();
		f->append("var %s:%s", field.getNameC(), getFieldType(field));
		if(field.getArray() || field.getType() == FT_STRING || field.getType() == FT_USER)
			f->append(" = new %s();", getFieldType(field));
		else
			f->append(";");
		f->end();
	}
	f->output("var __i__:uint = 0;");
	f->output("var __len__:uint = 0;");
	generateFieldContainerDeserializeCode(f, &m, "__r__", true);
	f->begin();
	f->append("__p__.%s(", m.getNameC());
	for(size_t i = 0; i < m.fields_.size(); i++)
	{
		Field& field = m.fields_[i];
		f->append("%s%s", field.getNameC(), (i == m.fields_.size()-1)?"":",");
	}
	f->append(");");
	f->end();
	f->recover();
	f->output("}");
}

static void generateServiceProxy(Service* s)
{
	CodeFile* f = openFile(s->getName() + "Proxy");
	if(s->super_)
		f->output("public interface %sProxy extends %sProxy", s->getNameC(), s->super_->getNameC());
	else
		f->output("public interface %sProxy", s->getNameC());
	f->output("{");
	f->indent();

	// methods.
	for(size_t i = 0; i < s->methods_.size(); i++)
		generateProxyAbstractMethod(f, s->methods_[i]);
	f->recover();
	f->output("}");
	closeFile(f);
}

static void generateServiceDispatcher(Service* s)
{
	CodeFile* f = openFile(s->getName() + "Dispatcher");
	if(s->super_)
		f->output("public class %sDispatcher extends %sDispatcher", s->getNameC(), s->super_->getNameC());
	else
		f->output("public class %sDispatcher", s->getNameC());
	f->output("{");
	f->indent();

	// deserializations.
	for(size_t i = 0; i < s->methods_.size(); i++)
		generateMethodDispatcher(f, s, s->methods_[i]);

	// dispatch function.
	f->output("public static function dispatch(r:IReader, p:%sProxy):void", s->getNameC());
	f->output("{");
	f->indent();
	f->output("var pid:uint = ProtocolReader.readUInt16(r);");
	f->output("switch(pid)");
	f->output("{");
	f->indent();
	std::vector<Method*> methods;
	s->getAllMethods(methods);
	for(size_t i = 0; i < methods.size(); i++)
	{
		Method* method = methods[i];
		f->output("case %d:", i);
		f->output("{");
		f->indent();
		f->output("%s(r, p);", method->getNameC());
		f->recover();
		f->output("}");
		f->output("break;");
	}
	f->output("default: throw \"invalid pid\";");
	f->recover();
	f->output("}");
	f->recover();
	f->output("}");
	f->recover();
	f->output("}");
	closeFile(f);
}

static void generateService(Service* s)
{
	generateServiceStub(s);
	generateServiceProxy(s);
	generateServiceDispatcher(s);
}

void ASGenerator::generate()
{
	// package dir.
	std::string pkgDirName = Compiler::inst().outputDir_ + Compiler::inst().fileStem_;
	if(!boost::filesystem::exists(pkgDirName))
		boost::filesystem::create_directory(pkgDirName);

	// iterate all definations.
	for(size_t i = 0; i < Compiler::inst().definitions_.size(); i++)
	{
		Definition* definition = Compiler::inst().definitions_[i];
		if(definition->getFile() != Compiler::inst().filename_)
			continue;
		// definations.
		if (definition->getEnum())
			generateEnum(definition->getEnum());
		else if (definition->getStruct())
			generateStruct(definition->getStruct());
		else if (definition->getService())
			generateService(definition->getService());
	}
}