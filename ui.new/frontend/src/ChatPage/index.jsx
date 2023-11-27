// Core
import { useTheme } from "@emotion/react";
import * as React from "react";

// @blueprintjs
import { Button, Icon, TextArea, Card} from "@blueprintjs/core";

const useChatTheme = () => {
  const palette = useTheme().palette;

  const generalTheme = {};

  return palette.mode === "dark"
    ? {
        ...generalTheme,
        brainIconColor: "#555",
        brainIconOpacity: 0.1,
        aiMessageBg: "#0F6894",
        userMessageBg: "#007067",
        text: "#fff",
      }
    : {
        ...generalTheme,
        brainIconColor: "#555",
        brainIconOpacity: 0.1,
        text: "#000",
        aiMessageBg: "#8ABBFF",
        userMessageBg: "#72CA9B",
      };
};

const Message = (props) => {
  const { userMessageBg, aiMessageBg, text } = useChatTheme();

  return (
    <div
      style={{
        display: "flex",
        flexDirection: props.ai ? "row" : "row-reverse",
        alignItems: "center",
        justifyContent: props.ai ? "left" : "right",
        padding: 4,
      }}>
      <Icon
        icon={props.ai ? "desktop" : "user"}
        size={30}
        style={{
          padding: "0px 9px 0px 9px",
          color: props.ai ? aiMessageBg : userMessageBg,
        }}></Icon>
      <Card
        style={{
          width: "max-content",
          padding: 5,
          color: text,
          borderRadius: 5,
          backgroundColor: props.ai ? aiMessageBg : userMessageBg,
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
        style={{ padingLeft: 10, height: "15vh", resize: "none" }}
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
            window.scrollTo(0, document.body.scrollHeight);
          }}
        />
      </div>
    </Card>
  );
};

const ChatPage = () => {
  const [messages, setMessages] = React.useState([
    { content: "Answer", ai: true },
  ]);

  return (
    <div style={{ padding: 30 }}>
      <MessageList messages={messages} />
      <Prompt messages={messages} setMessages={setMessages} />
    </div>
  );
};
export default ChatPage;
