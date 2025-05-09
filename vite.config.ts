import { defineConfig } from "vite";
import react from "@vitejs/plugin-react-swc";
import path from "path";

export default defineConfig(({ mode }) => ({
  // fixme here: does not work on ghpages
  base: mode === "ghpages" ? "/" : "/",
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