namespace useless {
unsigned int g_wasteful[400];
extern int g_notthere;
void Foo() {} // first Foo().

namespace wasteoftyme {
void Foo() {}
} // namespace wasteoftyme
} // namespace useless

namespace stupid {
void Foo() {}
void Bar() {}
void DoNothing(unsigned int) {}
} // namespace stupid

namespace useless {
void DoNothing(int) {}
} // namespace useless

using namespace useless;

void DoStuff() {
  stupid::Bar();
  Foo();
  using stupid::Foo;
  Foo();                        // stupid
  DoNothing(g_wasteful[3] + 1); // stupid::DoNothing(unsigned int)
}

void Foo() {}

using namespace stupid;

namespace comeon = useless::wasteoftyme;

void DoMoreStuff() {
  comeon::Foo();
  ::Foo(), useless::Foo(), comeon::Foo(),
      stupid::Foo(); // try uncommenting this line, solve the error
  Bar();             // why doesn't this line create an error?
  DoNothing(g_wasteful[3] + 1);
}

namespace useless {
void DoUselessStuff() { DoNothing(g_wasteful[3] + 1); }
} // namespace useless