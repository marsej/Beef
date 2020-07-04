using System;

namespace Tests
{
	class Reflection
	{
		[AttributeUsage(.Field, .ReflectAttribute)]
		struct AttrAAttribute : Attribute
		{
			public int32 mA;
			public float mB;
			public String mC;
			public String mD;

			public this(int32 a, float b, String c, String d)
			{
				PrintF("this: %p A: %d B: %f", this, a, (double)b);

				mA = a;
				mB = b;
				mC = c;
				mD = d;
			}
		}

		[AttributeUsage(.Field)]
		struct AttrBAttribute : Attribute
		{
			public int32 mA;
			public float mB;

			public this(int32 a, float b)
			{
				mA = a;
				mB = b;
			}
		}

		[AttributeUsage(.Class | .Method, .ReflectAttribute)]
		struct AttrCAttribute : Attribute
		{
			public int32 mA;
			public float mB;

			public this(int32 a, float b)
			{
				mA = a;
				mB = b;
			}
		}


		[Reflect]
		class ClassA
		{
			[AlwaysInclude, AttrC(71, 72)]
			static float StaticMethodA(int32 a, int32 b, float c, ref int32 d, ref StructA sa)
			{
				d += a + b;
				sa.mA += a;
				sa.mB += b;
				return a + b + c;
			}

			[AlwaysInclude]
			float MemberMethodA(int32 a, int32 b, float c)
			{
				return a + b + c;
			}

			public virtual int GetA(int32 a)
			{
				return a + 1000;
			}

			public virtual int GetB(int32 a)
			{
				return a + 3000;
			}
		}

		[Reflect, AlwaysInclude(IncludeAllMethods=true)]
		struct StructA
		{
			public int mA;
			public int mB;

			int GetA(int a)
			{
				return a + mA * 100;
			}

			int GetB(int a) mut
			{
				mB += a;
				return a + mA * 100;
			}
		}

		class ClassA2 : ClassA
		{
			public override int GetA(int32 a)
 			{
				 return a + 2000;
			}

			public override int GetB(int32 a)
			{
				 return a + 4000;
			}
		}

		[Reflect(.All), AttrC(1, 2)]
		class ClassB
		{
			[AttrA(11, 22, "StrA", "StrB")]
			public int mA = 1;
			[AttrB(44, 55)]
			public int mB = 2;
			public int mC = 3;
			public String mStr = "ABC";
		}

		[Reflect(.Type)]
		class ClassC
		{
			[AttrA(11, 22, "StrA", "StrC")]
			public int mA = 1;
			[AttrB(44, 55)]
			public int mB = 2;
			public int mC = 3;
			public float mD = 4;
		}

		[Test]
		static void TestTypes()
		{
			Type t = typeof(int32);
			Test.Assert(t.InstanceSize == 4);
			t = typeof(int64);
			Test.Assert(t.InstanceSize == 8);
		}

		[Test]
		static void TestA()
		{
			ClassA ca = scope ClassA();

			ClassA2 ca2 = scope ClassA2();

			Test.Assert(ca2.GetA(9) == 2009);

			int methodIdx = 0;
			var typeInfo = typeof(ClassA);
			for (let methodInfo in typeInfo.GetMethods())
			{
				switch (methodIdx)
				{
				case 0:
					StructA sa = .() { mA = 1, mB = 2 };

					Test.Assert(methodInfo.Name == "StaticMethodA");
					int32 a = 0;
					var result = methodInfo.Invoke(null, 100, (int32)20, 3.0f, &a, &sa).Get();
					Test.Assert(a == 120);
					Test.Assert(sa.mA == 101);
					Test.Assert(sa.mB == 22);
					Test.Assert(result.Get<float>() == 123);
					result.Dispose();

					result = methodInfo.Invoke(.(), .Create(100), .Create((int32)20), .Create(3.0f), .Create(&a), .Create(&sa)).Get();
					Test.Assert(a == 240);
					Test.Assert(sa.mA == 201);
					Test.Assert(sa.mB == 42);
					Test.Assert(result.Get<float>() == 123);
					result.Dispose();

					let attrC = methodInfo.GetCustomAttribute<AttrCAttribute>().Get();
					Test.Assert(attrC.mA == 71);
					Test.Assert(attrC.mB == 72);
				case 1:
					Test.Assert(methodInfo.Name == "MemberMethodA");
					var result = methodInfo.Invoke(ca, 100, (int32)20, 3.0f).Get();
					Test.Assert(result.Get<float>() == 123);
					result.Dispose();

					result = methodInfo.Invoke(.Create(ca), .Create(100), .Create((int32)20), .Create(3.0f)).Get();
					Test.Assert(result.Get<float>() == 123);
					result.Dispose();
				case 2:
					Test.Assert(methodInfo.Name == "GetA");
					var result = methodInfo.Invoke(ca, 123).Get();
					Test.Assert(result.Get<int>() == 1123);
					result.Dispose();
					result = methodInfo.Invoke(ca2, 123).Get();
					Test.Assert(result.Get<int>() == 2123);
					result.Dispose();
					result = methodInfo.Invoke(.Create(ca2), .Create(123)).Get();
					Test.Assert(result.Get<int>() == 2123);
					result.Dispose();
				case 3:
					Test.Assert(methodInfo.Name == "__BfCtor");
					Test.Assert(methodInfo.IsConstructor);
				case 4:
					Test.FatalError(); // Shouldn't have any more
				}

				methodIdx++;
			}
		}

		[Test]
		static void TestB()
		{
			ClassB cb = scope ClassB();
			int fieldIdx = 0;
			for (let fieldInfo in cb.GetType().GetFields())
			{
				switch (fieldIdx)
				{
				case 0:
					Test.Assert(fieldInfo.Name == "mA");
					var attrA = fieldInfo.GetCustomAttribute<AttrAAttribute>().Get();
					Test.Assert(attrA.mA == 11);
					Test.Assert(attrA.mB == 22);
					Test.Assert(attrA.mC == "StrA");
					Test.Assert(attrA.mD == "StrB");
				}

				fieldIdx++;
			}

			var fieldInfo = cb.GetType().GetField("mC").Value;
			int cVal = 0;
			fieldInfo.GetValue(cb, out cVal);
			fieldInfo.SetValue(cb, cVal + 1000);
			Test.Assert(cb.mC == 1003);

			Variant variantVal = Variant.Create(123);
			fieldInfo.SetValue(cb, variantVal);
			Test.Assert(cb.mC == 123);

			fieldInfo = cb.GetType().GetField("mStr").Value;
			String str = null;
			fieldInfo.GetValue(cb, out str);
			Test.Assert(str == "ABC");
			fieldInfo.SetValue(cb, "DEF");
			Test.Assert(cb.mStr == "DEF");
		}

		[Test]
		static void TestC()
		{
			let attrC = typeof(ClassB).GetCustomAttribute<AttrCAttribute>().Get();
			Test.Assert(attrC.mA == 1);
			Test.Assert(attrC.mB == 2);
		}

		[Test]
		static void TestD()
		{
			StructA sa = .() { mA = 12, mB = 23 };
			var typeInfo = typeof(StructA);

			int methodIdx = 0;
			for (let methodInfo in typeInfo.GetMethods())
			{
				switch (methodIdx)
				{
				case 0:
					Test.Assert(methodInfo.Name == "GetA");

					var result = methodInfo.Invoke(sa, 34).Get();
					Test.Assert(result.Get<int32>() == 1234);
					result.Dispose();

					result = methodInfo.Invoke(&sa, 34).Get();
					Test.Assert(result.Get<int32>() == 1234);
					result.Dispose();

					Variant saV = .Create(sa);
					defer saV.Dispose();
					result = methodInfo.Invoke(saV, .Create(34));
					Test.Assert(result.Get<int32>() == 1234);
					result.Dispose();

					result = methodInfo.Invoke(.Create(&sa), .Create(34));
					Test.Assert(result.Get<int32>() == 1234);
					result.Dispose();
				case 1:
					Test.Assert(methodInfo.Name == "GetB");

					var result = methodInfo.Invoke(sa, 34).Get();
					Test.Assert(result.Get<int32>() == 1234);
					Test.Assert(sa.mB == 23);
					result.Dispose();

					result = methodInfo.Invoke(&sa, 34).Get();
					Test.Assert(result.Get<int32>() == 1234);
					Test.Assert(sa.mB == 57);
					result.Dispose();

					Variant saV = .Create(sa);
					defer saV.Dispose();
					result = methodInfo.Invoke(saV, .Create(34));
					Test.Assert(result.Get<int32>() == 1234);
					Test.Assert(sa.mB == 57);
					result.Dispose();

					result = methodInfo.Invoke(.Create(&sa), .Create(34));
					Test.Assert(result.Get<int32>() == 1234);
					Test.Assert(sa.mB == 91);
					result.Dispose();

				case 2:
					Test.Assert(methodInfo.Name == "__BfCtor");
				case 3:
					Test.Assert(methodInfo.Name == "__Equals");
				case 4:
					Test.Assert(methodInfo.Name == "__StrictEquals");
				default:
					Test.FatalError(); // Shouldn't have any more
				}

				methodIdx++;
			}
		}
	}
}
