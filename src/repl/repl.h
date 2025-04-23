#pragma once

namespace repl 
{
	// Starts an interactive terminal.
	int Start();

	// Runs a file.
	int Run(const char* p_fileName);
}