import { defineConfig } from "vite";
import react from "@vitejs/plugin-react-swc";
import path from "path";

export default defineConfig(({ mode }) => ({
<<<<<<< HEAD
  base: mode === "ghpages" ? "nepdate/" : "/",
=======
>>>>>>> 7912cbbd8047db138868214562356389812b0c13
  build: {
    outDir: mode === "ghpages" ? "docs" : "dist",
    emptyOutDir: true,
  },
  server: {
    host: "::",
    port: 8080,
  },
  preview: {
    port: 8080,
  },
  plugins: [
    react(),
  ],
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"),
    },
  },
}));