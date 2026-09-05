TEMPLATE = subdirs
SUBDIRS = Client Server DataBase Singleton UnitTests
UnitTests.depends = Server
