/*
 * Copyright (C) 2024 khumnath
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.15

// Theme.qml
QtObject {
    id: theme
    property bool isDark: false

    // === Backgrounds ===
    readonly property color primaryBg: isDark ? "#111827" : "#f3f4f6"
    readonly property color secondaryBg: isDark ? "#1f2937" : "#ffffff"
    readonly property color tertiaryBg: isDark ? "#374151" : "#f1f5f9"
    readonly property color inputBg: isDark ? "#1f2937" : "#ffffff"

    // === Text Colors ===
    readonly property color primaryText: isDark ? "#f9fafb" : "#111827"
    readonly property color secondaryText: isDark ? "#9ca3af" : "#6b7280"
    readonly property color accentText: isDark ? "#38bdf8" : "#2563eb"

    // === Calendar Specific ===
    readonly property color saturdayText: isDark ? "#fca5a5" : "#dc2626"
    readonly property color purnimaText: isDark ? "#fbbf24" : "#d97706"

    // === Web Style Headers ===
    readonly property color normalHeaderBg: isDark ? "#312e81" : "#3b82f6"
    readonly property color saturdayHeaderBg: isDark ? "#7f1d1d" : "#dc2626"
    readonly property color headerPrimaryText: "#ffffff"

    // === Accent / Interactive ===
    readonly property color accent: isDark ? "#3b82f6" : "#2563eb"
    readonly property color accentHover: isDark ? "#60a5fa" : "#3b82f6"
    readonly property color accentDark: isDark ? "#1d4ed8" : "#1d4ed8"

    // === Borders ===
    readonly property color borderColor: isDark ? "#374151" : "#e5e7eb"
    readonly property color todayBorder: isDark ? "#6366f1" : "#3b82f6"
    readonly property color holidayBorder: isDark ? "#b45309" : "#f59e0b"
    readonly property color saturdayBorder: isDark ? "#7f1d1d" : "#fca5a5"

    // === Highlighted Days ===
    readonly property color todayBg: isDark ? "transparent" : "#eff6ff"
    readonly property color saturdayBg: isDark ? "#451a1a" : "#fef2f2"
    readonly property color adDayText: secondaryText

    // === Modal / Dialogs ===
    readonly property color modalHeaderText: primaryText
    readonly property color modalButtonBg: tertiaryBg
    readonly property color modalButtonText: primaryText
    readonly property color infoText: isDark ? "#fbbf24" : "#d97706"
}
