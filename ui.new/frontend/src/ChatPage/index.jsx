// Core
import { useTheme } from "@emotion/react";
import * as React from "react";

// @blueprintjs
import { Button, Icon, TextArea, Card } from "@blueprintjs/core";

const useChatTheme = () => {
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
        brainIconOpacity: 0.1,
        aiMessageBg: "#36495e",
        userMessageBg: "#313F4C",
        iconBackgroundColor: "#999",
        text: "#fff",
        name: "#aaa",
      }
    : {
        ...generalTheme,
        brainIconColor: "#555",
        brainIconOpacity: 0.1,
        text: "#000",
        iconBackgroundColor: "#333",
        aiMessageBg: "#bcd9ff",
        userMessageBg: "#bcffda",
        name: "#444",
      };
};

const Message = (props) => {
  const {
    name,
    userMessageBg,
    aiMessageBg,
    iconBackgroundColor,
    text,
    iconSize,
    iconPadding,
    userNameSize,
  } = useChatTheme();

  return (
    <div
      style={{
        display: "flex",
        flexDirection: "column",
        alignItems: props.ai ? "start" : "end",
        padding: 5,
      }}>
      <div
        style={{
          display: "flex",
          flexDirection: "row",
          width: "maxContent",
          alignItems: "center",
        }}>
        {props.ai || (
          <div
            style={{
              fontSize: userNameSize,
              padding: 5,
              color: name,
            }}>
            You
          </div>
        )}
        <Icon
          icon={props.ai ? "desktop" : "user"}
          size={iconSize}
          style={{
            marginBottom: 5,
            padding: iconPadding,
            backgroundColor: iconBackgroundColor,
            borderRadius: "50%",
            color: props.ai ? aiMessageBg : userMessageBg,
            display: "block",
          }}
        />
        {props.ai && (
          <div
            style={{
              fontSize: userNameSize,
              padding: 5,
              color: name,
            }}>
            Assistant
          </div>
        )}
      </div>
      <Card
        style={{
          width: "max-content",
          padding: "12px 20px 12px 20px",
          color: text,
          backgroundColor: props.ai ? aiMessageBg : userMessageBg,
          maxWidth: "100%"
        }}>
        <code style={{ fontSize: 13 }}>{props.content}</code>
      </Card>
    </div>
  );
};

const MessageList = ({ messages }) => {
  const { brainIconOpacity, brainIconColor } = useChatTheme();

  const defaultStyle = {
    display: "flex",
    minHeight: "60vh",
    marginBottom: 20,
  };

  return messages.length === 0 ? (
    <Card
      elevation={4}
      style={{
        ...defaultStyle,
        alignItems: "center",
        justifyContent: "center",
      }}>
      <Icon
        icon="desktop"
        size={100}
        color={brainIconColor}
        style={{
          padding: 10,
          opacity: brainIconOpacity,
        }}
      />
    </Card>
  ) : (
    <Card
      elevation={4}
      style={{
        ...defaultStyle,
        flexDirection: "column",
        justifyContent: "end",
      }}>
      {messages.map((msg, i) => (
        <Message key={"msg-" + i} content={msg.content} ai={msg.ai} />
      ))}
    </Card>
  );
};

const Prompt = ({ messages, setMessages }) => {
  const [currentMsg, setCurrentMsg] = React.useState("");

  React.useEffect(() => {
    window.scrollTo(0, document.body.scrollHeight);
  }, [messages]);

  return (
    <Card
      elevation={4}
      style={{
        display: "flex",
        justifyContent: "space-between",
        alignItems: "center",
        padding: 10,
      }}>
      <TextArea
        id="current-msg-prompt"
        asyncControl={true}
        fill
        style={{ padingLeft: 10, height: "8vh", resize: "none" }}
        value={currentMsg}
        placeholder="Ask me anything..."
        onChange={(e) => setCurrentMsg(e.target.value)}
      />
      <div style={{ paddingLeft: 10 }}>
        <Button
          icon="send-message"
          disabled={currentMsg.length === 0}
          onClick={() => {
            setMessages([
              ...messages,
              {
                content: currentMsg,
                ai: false,
              },
            ]);
            setCurrentMsg("");
          }}
        />
      </div>
    </Card>
  );
};

const ChatPage = () => {
  const [messages, setMessages] = React.useState([
    {
      content:
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum et fringilla urna, a semper leo. Suspendisse feugiat hendrerit suscipit. Pellentesque commodo condimentum nulla, quis venenatis elit pretium sed. Donec quis turpis in dui ornare laoreet eu suscipit sapien. Quisque lobortis dapibus ante, a iaculis augue rhoncus vel. Fusce a hendrerit nisi. Donec ac laoreet sapien, in finibus magna. Nulla libero urna, malesuada",
      ai: true,
    },
  ]);

  return (
    <div style={{ padding: 30, maxWidth: "100vh", overflow: "hidden" }}>
      <MessageList messages={messages} />
      <Prompt messages={messages} setMessages={setMessages} />
    </div>
  );
};
export default ChatPage;
