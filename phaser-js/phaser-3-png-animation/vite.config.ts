import fs from "fs";
import path from "path";
import { Alias, defineConfig } from "vite";

const NODE_MODULES_OVERRIDE_PATH = path.resolve(
  __dirname,
  "node_modules_override",
);

const NODE_MODULES_OVERRIDE_PHASER_PATH = path.resolve(NODE_MODULES_OVERRIDE_PATH, 'phaser');
const NODE_MODULES_OVERRIDE_PHASER_JS_PATH = path.resolve(NODE_MODULES_OVERRIDE_PHASER_PATH, 'dist/phaser.js');

const resolveAlias: Alias[] = [
  { find: "@", replacement: path.resolve(__dirname, "src") },
  { find: "~", replacement: path.resolve(__dirname, "src/game") },
];

if (fs.existsSync(NODE_MODULES_OVERRIDE_PHASER_JS_PATH)) {
    console.log(`Using tree-shaken Phaser override at "${NODE_MODULES_OVERRIDE_PHASER_JS_PATH}"`);
    resolveAlias.push({ find: 'phaser', replacement: `${NODE_MODULES_OVERRIDE_PHASER_JS_PATH}`});
} else {
    console.log(`Tree-shaken Phaser override at "${NODE_MODULES_OVERRIDE_PHASER_JS_PATH}" not found, using default one`);
}

const phasermsg = () => {
  return {
    name: "phasermsg",
    buildStart() {
      process.stdout.write(`Building for production...\n`);
    },
    buildEnd() {
      const line = "---------------------------------------------------------";
      const msg = `❤️❤️❤️ Tell us about your game! - games@phaser.io ❤️❤️❤️`;
      process.stdout.write(`${line}\n${msg}\n${line}\n`);

      process.stdout.write(`🕚️ Wait few more seconds...\n`);
    },
  };
};

export default defineConfig({
  base: "./",
  resolve: {
    alias: resolveAlias,
  },
  // logLevel: 'warning',
  logLevel: "warn",
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          phaser: ["phaser"],
        },
      },
    },
    minify: "terser",
    terserOptions: {
      compress: {
        passes: 2,
      },
      mangle: true,
      format: {
        comments: false,
      },
    },
  },
  server: {
    port: 8080,
  },
  plugins: [phasermsg()],
});
