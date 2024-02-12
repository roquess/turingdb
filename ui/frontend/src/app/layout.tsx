import type { Metadata } from 'next'
import '@blueprintjs/icons/lib/css/blueprint-icons.css'
import '@blueprintjs/core/lib/css/blueprint.css'
import '@blueprintjs/select/lib/css/blueprint-select.css'
import './globals.css'
import LayoutSidebar from '@/components/layout/LayoutSidebar'
import LayoutTopbar from '@/components/layout/LayoutTopbar'
import { Classes } from '@blueprintjs/core'

export const metadata: Metadata = {
  title: 'Turing biosystems',
  // TODO: Add description
  description: '',
}

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <body
        className={`${Classes.DARK} flex min-h-screen overflow-x-hidden bg-grey-900 font-sans text-content-primary`}
      >
        <LayoutSidebar />
        <div className="flex flex-grow flex-col overflow-x-hidden">
          <LayoutTopbar />
          <main className="flex flex-grow flex-col overflow-x-hidden">{children}</main>
        </div>
      </body>
    </html>
  )
}
