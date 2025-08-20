import QtQuick 2.15

// Theme.qml
QtObject {
    id: theme
    property bool isDark: false

    // === Backgrounds ===
    readonly property color primaryBg: isDark ? "#0f172a" : "#f9fafb"
    readonly property color secondaryBg: isDark ? "#1e293b" : "#ffffff"
    readonly property color tertiaryBg: isDark ? "#334155" : "#f1f5f9"
    readonly property color inputBg: isDark ? "#1e293b" : "#ffffff"

    // === Text Colors ===
    readonly property color primaryText: isDark ? "#e2e8f0" : "#0f172a"
    readonly property color secondaryText: isDark ? "#94a3b8" : "#475569"
    readonly property color accentText: isDark ? "#67e8f9" : "#0e7490"

    // === Calendar Specific ===
    readonly property color saturdayText: isDark ? "#fecaca" : "#F86262"
    readonly property color purnimaText: isDark ? "#fde68a" : "#92400e"

    // === Headers ===
    readonly property color headerBg: isDark ? "#0f172a" : "#ffffff"
    readonly property color headerPrimaryText: isDark ? "#f8fafc" : "#1e293b"
    readonly property color headerSecondaryText: isDark ? "#cbd5e1" : "#64748b"
    readonly property color headerDivider: isDark ? "#475569" : "#cbd5e1"

    // === Accent / Interactive ===
    readonly property color accent: isDark ? "#22d3ee" : "#0ea5e9"
    readonly property color accentHover: isDark ? "#67e8f9" : "#38bdf8"
    readonly property color accentDark: isDark ? "#0e7490" : "#0369a1"

    // === Borders ===
    readonly property color borderColor: isDark ? "#334155" : "#e2e8f0"
    readonly property color todayBorder: isDark ? "#0ea5e9" : "#0284c7"
    readonly property color saturdayBorder: isDark ? "#334155" : "#fda4af"

    // === Highlighted Days ===
    readonly property color todayBg: isDark ? "#082f49" : "#f0f9ff"
    readonly property color saturdayBg: isDark ? "#6E3233" : "#FEDDDF"
    readonly property color adDayText: isDark ? "#BBCFFA" : "#6E9BFD"

    // === Modal / Dialogs ===
    readonly property color modalHeaderText: headerPrimaryText
    readonly property color modalButtonBg: tertiaryBg
    readonly property color modalButtonText: headerPrimaryText
    readonly property color infoText: isDark ? "#FFBF00" : "#9B7606"
}
