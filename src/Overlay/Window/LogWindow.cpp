#include "LogWindow.h"

void LogWindow::BeforeDraw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
}

void LogWindow::Draw()
{
	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	m_filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (copy)
	{
		ImGui::LogToClipboard();
		AddLog("[system] Log has been copied to clipboard\n");
	}

	if (m_filter.IsActive())
	{
		const char* bufBegin = m_buf.begin();
		const char* line = bufBegin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* lineEnd = (line_no < m_lineOffsets.Size) ? bufBegin + m_lineOffsets[line_no] : NULL;
			if (m_filter.PassFilter(line, lineEnd))
			{
				ImGui::TextUnformatted(line, lineEnd);
				// causes a line limit in the logging for some reason
				// ImGui::TextWrapped(line, line_end);
			}
			line = lineEnd && lineEnd[1] ? lineEnd + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(m_buf.begin());
		// Causes a line limit in the logging for some reason
		// ImGui::TextWrapped(Buf.begin());
	}

	// Handle automatic scrolling
	if (m_prevScrollMaxY < ImGui::GetScrollMaxY())
	{
		// Scroll down automatically only if we didnt scroll up or we closed the window
		if (ImGui::GetScrollY() >= m_prevScrollMaxY - 5)
		{
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
		}
		m_prevScrollMaxY = ImGui::GetScrollMaxY();
	}

	ImGui::EndChild();
}

void LogWindow::Clear()
{
	m_buf.clear();
	m_lineOffsets.clear();
}

void LogWindow::AddLog(const char* fmt, ...)
{
	if (!m_loggingEnabled)
	{
		return;
	}

	int oldSize = m_buf.size();
	va_list args;
	va_start(args, fmt);
	m_buf.appendfv(fmt, args);
	va_end(args);
	for (int newSize = m_buf.size(); oldSize < newSize; oldSize++)
	{
		if (m_buf[oldSize] == '\n')
		{
			m_lineOffsets.push_back(oldSize);
		}
	}
}

void LogWindow::ToFile(FILE* file) const
{
	fprintf(file, "%s", m_buf.begin());
}

void LogWindow::SetLogging(bool value)
{
	m_loggingEnabled = value;
}

bool LogWindow::IsLoggingOn() const
{
	return m_loggingEnabled;
}
