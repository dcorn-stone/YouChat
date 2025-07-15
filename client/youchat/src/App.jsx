import { useState, useEffect} from "react";
import reactLogo from "./assets/react.svg";
import { invoke } from "@tauri-apps/api/core";
import AuthPage from "./pages/AuthPage"; 
import ServerAddr from "./pages/ServerAddr.jsx"

import "./App.css";

function App() {
  const [configured, setConfigured] = useState(false);

  // useEffect(() => {
  //   const address = localStorage.getItem("server_address");
  //   const port = localStorage.getItem("server_port");
  //   if (address && port) {
  //     setConfigured(true);
  //   }
  // }, []);

  return (
    <div>
      {!configured ? (
        <ServerAddr onConfigured={() => setConfigured(true)} />
      ) : (
        <AuthPage />
      )}
    </div>
  );
}


export default App;
