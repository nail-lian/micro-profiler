cmake_minimum_required(VERSION 2.8)

include(test)

set(FRONTEND_TESTS_SOURCES
	ColumnsModelTests.cpp
	FilterViewTests.cpp
	FrontendManagerTests.cpp
	FrontendTests.cpp
	FunctionListPersistence.cpp
	FunctionListPiechartTests.cpp
	FunctionListTests.cpp
	MarshalledServerTests.cpp
	mocks.cpp
	OrderedViewTests.cpp
	ProcessListTests.cpp
	SerializationTests.cpp
	SymbolResolverTests.cpp
	ThreadsModelTests.cpp
)

add_library(frontend.tests SHARED ${FRONTEND_TESTS_SOURCES})

target_link_libraries(frontend.tests frontend.lib test-helpers logger common mt agge.text)
