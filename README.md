# Bikram Sambat Calendar

A modern React + TypeScript web application for converting and viewing Nepali (Bikram Sambat) and Gregorian dates, with tithi calculation, event/holiday display, and a beautiful calendar UI.

## Features

- Convert between Bikram Sambat (BS) and Gregorian (AD) dates
- View Nepali and English months, days, and years
- Calculate lunar tithi for any date
- See major Nepali festivals and holidays
- Responsive, accessible, and print-friendly calendar UI
- Toggle between Nepali and English language
- "Show in Calendar" and event modal support
- Built with [Vite](https://vitejs.dev/), [React](https://react.dev/), [Tailwind CSS](https://tailwindcss.com/), and [shadcn/ui](https://ui.shadcn.com/)

## Getting Started

### Prerequisites

- [Node.js](https://nodejs.org/) (v18+ recommended)
- [Yarn](https://yarnpkg.com/) or [npm](https://www.npmjs.com/)

### Installation

```sh
git clone https://github.com/yourusername/nepdate.git
cd nepdate
yarn install
# or
npm install
```

### Development

Start the local development server:

```sh
yarn dev
# or
npm run dev
```

Open [http://localhost:8080](http://localhost:8080) in your browser.

> **Note:** If you encounter a "too many open files" error, you may need to increase your system's file watch limit. See [this StackOverflow answer](https://stackoverflow.com/questions/53930305/nodemon-error-system-limit-for-number-of-file-watchers-reached) for help.

### Local Production Build

Build and preview locally:

```sh
yarn build
yarn preview
# or
npm run build
npm run preview
```

This will output the production build to the `dist/` directory, which is **not pushed to GitHub**.

### Build for GitHub Pages

To build for GitHub Pages (output to `docs/` and set correct base path):

```sh
yarn build-gh
# or
npm run build-gh
```

Preview the GitHub Pages build locally:

```sh
yarn preview-gh
# or
npm run preview-gh
```

This will output the production build to the `docs/` directory, which **should be committed and pushed** to your repository for GitHub Pages deployment.

### Deploy to GitHub Pages

1. Commit and push the `docs` folder to your repository.
2. In your repository settings, set GitHub Pages to serve from the `docs` folder on the `main` branch.
3. Your site will be available at `https://<username>.github.io/nepdate/`.

## Project Structure

```
├── bikram-calendar/      # Bikram Sambat calendar calculation library 
├── src/                  # React app source code
│   ├── components/       # React components
│   ├── data/             # Event and holiday data
│   ├── hooks/            # Custom React hooks
│   ├── utils/            # Utility functions
│   └── index.css         # Main CSS (Tailwind)
├── docs/                 # Production build output for GitHub Pages (should be pushed)
├── dist/                 # Production build output for local preview (not pushed)
├── index.html            # Main HTML entry
├── package.json
├── postcss.config.js
├── tailwind.config.ts
└── vite.config.ts
```

## Scripts

| Command         | Description                                 |
|-----------------|---------------------------------------------|
| `dev`           | Start local dev server                      |
| `build`         | Build for local production (`dist/`)        |
| `preview`       | Preview local production build              |
| `build-gh`      | Build for GitHub Pages (`docs/`)            |
| `preview-gh`    | Preview GitHub Pages build                  |
| `lint`          | Run ESLint                                  |

## Notes on Build Outputs

- The `dist/` directory is for local production builds and **should not be pushed to Git**.
- The `docs/` directory is for GitHub Pages deployment and **should be committed and pushed**.

## License

This project is licensed under the [GPL v3 or later](https://www.gnu.org/licenses/gpl-3.0.en.html).

---

**Made with love by Khumnath.**