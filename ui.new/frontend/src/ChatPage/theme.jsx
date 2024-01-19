import { useTheme } from "@emotion/react";

export const useChatTheme = () => {
  const palette = useTheme().palette;

  const generalTheme = {
    iconSize: 14,
    iconPadding: 4,
    userNameSize: 13,
  };

  return palette.mode === "dark"
    ? {
        ...generalTheme,
        brainIconColor: "#555",
        brainIconOpacity: 0.4,
        aiMessageBg: "#36495e",
        userMessageBg: "#313F4C",
        iconBackgroundColor: "#999",
        text: "#fff",
        name: "#aaa",
      }
    : {
        ...generalTheme,
        brainIconColor: "#555",
        brainIconOpacity: 0.2,
        text: "#000",
        iconBackgroundColor: "#333",
        aiMessageBg: "#bcd9ff",
        userMessageBg: "#bcffda",
        name: "#444",
      };
};
